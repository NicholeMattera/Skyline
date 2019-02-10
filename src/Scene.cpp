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

#include "Scene.hpp"

namespace skyline {
    Scene::Scene() {}
    Scene::~Scene() {}

    void Scene::handleButton(u32 kDown) {}

    void Scene::render(Rect rect, double dTime) {
        for (auto const& view : this->subviews) {
            if (!view->hidden) {
                s16 x = rect.origin.x + view->frame.origin.x;
                s16 y = rect.origin.y + view->frame.origin.y;
                s16 w = view->frame.size.w;
                s16 h = view->frame.size.h;
                view->render(SLRectMake(x, y, w, h), dTime);
            }
        }
    }

    void Scene::addSubView(View * view) {
        view->superview = NULL;
        this->subviews.push_back(view);
    }

    void Scene::removeSubView(View * view) {
        view->superview = NULL;
        this->subviews.remove(view);
    }
}