//
// Skyline Example
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

#include "ExampleScene.hpp"

using namespace skyline;

namespace skylineExample {
    ExampleScene::ExampleScene() : Scene() {
        _headerView = new HeaderView("Skyline Example", "sdmc:/dark/icon.png");
        this->addSubView(_headerView);

        _footerView = new FooterView();
        this->addSubView(_footerView);
    }

    ExampleScene::~ExampleScene() {
        if (_headerView != NULL) {
            delete _headerView;
            _headerView = NULL;
        }

        if (_footerView != NULL) {
            delete _footerView;
            _footerView = NULL;
        }
    }

    void ExampleScene::handleButton(u32 kDown) {
        if (kDown & KEY_PLUS) {
            Application::sharedApplication->popScene();
        }
    }
}