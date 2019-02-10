//
// Skyline
// Copyright (C) 2019 Steven Mattera
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//

#include "Application.hpp"
#include "Draw.hpp"

namespace skyline {
    Application::Application() {
        Application::sharedApplication = this;

        this->_window = nwindowGetDefault();
        
        framebufferCreate(&this->_framebuffer, this->_window, FB_WIDTH, FB_HEIGHT, PIXEL_FORMAT_RGBA_8888, 2);
        framebufferMakeLinear(&this->_framebuffer);

        setsysInitialize();
        setsysGetColorSetId(&colorSetId);
        setsysExit();

        this->backgroundColor = (colorSetId == ColorSetId_Light) ? SLColorMake(235, 235, 235, 255) : SLColorMake(45, 45, 45, 255);
        this->_currentScene = NULL;
    }

    Application::~Application() {
        Application::sharedApplication = NULL;
        _sceneStack.clear();
        framebufferClose(&this->_framebuffer);
    }

    void Application::setRootScene(Scene * scene) {
        this->_sceneStack.clear();
        this->pushScene(scene);
    }

    void Application::pushScene(Scene * scene) {
        this->_sceneStack.push_back(scene);
        this->_currentScene = scene;
    }

    void Application::popScene() {
        this->_sceneStack.pop_back();

        if (_sceneStack.size() == 0) {
            this->_currentScene = NULL;
        } else {
            this->_currentScene = this->_sceneStack.back();
        }
    }

    // Public Methods

    void Application::start() {
        while (appletMainLoop()) {
            hidScanInput();
            u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

            this->_currentScene->handleButton(kDown);
            if (this->_currentScene == NULL)
                break;

            Draw::framebuffer = (u8 *) framebufferBegin(&this->_framebuffer, &Draw::stride);
            Draw::drawFilledRectangle(SLRectMake(0, 0, FB_WIDTH, FB_HEIGHT), this->backgroundColor);

            // TODO: Calculate delta time.
            this->_currentScene->render(SLRectMake(0, 0, FB_WIDTH, FB_HEIGHT), 0);

            framebufferEnd(&this->_framebuffer);
        }
    }
}
