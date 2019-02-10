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

#pragma once

#include <switch.h>
#include <vector>
#include "Scene.hpp"
#include "Types.hpp"

namespace skyline {
    class Application {
        public:
            static inline Application * sharedApplication;

            Application();
            ~Application();
            
            void setRootScene(Scene * scene);
            void pushScene(Scene * scene);
            void popScene();

            void start();

            ColorSetId colorSetId;
            Color backgroundColor;
        private:
            NWindow * _window;
            Framebuffer _framebuffer;
            std::vector<Scene *> _sceneStack;
            Scene * _currentScene;
    };
}
