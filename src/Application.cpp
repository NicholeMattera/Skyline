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

        socketInitializeDefault();
        nxlinkStdio();
        romfsInit();
        Draw::drawInit();

        setsysInitialize();
        setsysGetColorSetId(&colorSetId);
        setsysExit();

        this->backgroundColor = (colorSetId == ColorSetId_Light) ? SLColorMake(235, 235, 235, 255) : SLColorMake(45, 45, 45, 255);
        this->_currentScene = NULL;
    }

    Application::~Application() {
        Application::sharedApplication = NULL;
        _sceneStack.clear();
        Draw::drawExit();
        romfsExit();
        socketExit();
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

            #ifdef USE_SDL
                SDL_SetRenderDrawColor(SceneDirector::renderer, this->backgroundColor.r, this->backgroundColor.g, this->backgroundColor.b, this->backgroundColor.a);
                SDL_RenderClear(Draw::renderer);
            #else
                Draw::currentFramebuffer = (u8 *) framebufferBegin(&Draw::framebuffer, &Draw::stride);
                Draw::drawFilledRectangle(SLRectMake(0, 0, FB_WIDTH, FB_HEIGHT), this->backgroundColor);
            #endif

            // TODO: Calculate delta time.
            this->_currentScene->render(SLRectMake(0, 0, FB_WIDTH, FB_HEIGHT), 0);

            #ifdef USE_SDL
                SDL_RenderPresent(Draw::renderer);
            #else
                framebufferEnd(&Draw::framebuffer);
            #endif
        }
    }
}
