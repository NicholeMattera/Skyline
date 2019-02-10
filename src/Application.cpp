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
        this->_window = nwindowGetDefault();
        
        framebufferCreate(&this->_framebuffer, this->_window, FB_WIDTH, FB_HEIGHT, PIXEL_FORMAT_RGBA_8888, 2);
        framebufferMakeLinear(&this->_framebuffer);

        setsysInitialize();
        setsysGetColorSetId(&Application::colorSetId);
        setsysExit();

        this->backgroundColor = (Application::colorSetId == ColorSetId_Light) ? SLColorMake(235, 235, 235, 255) : SLColorMake(45, 45, 45, 255);
    }

    Application::~Application() {
        framebufferClose(&this->_framebuffer);
    }

    // Public Methods

    void Application::start() {
        while (appletMainLoop()) {
            hidScanInput();

            u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
            if (kDown & KEY_PLUS)
                break;

            Draw::framebuffer = (u8 *) framebufferBegin(&this->_framebuffer, &Draw::stride);
            Draw::drawFilledRectangle(SLRectMake(0, 0, FB_WIDTH, FB_HEIGHT), this->backgroundColor);

            if (currentScene != NULL) {
                currentScene->handleButton(kDown);
                currentScene->render(SLRectMake(0, 0, FB_WIDTH, FB_HEIGHT), 0);
            }

            framebufferEnd(&this->_framebuffer);
        }
    }
}
