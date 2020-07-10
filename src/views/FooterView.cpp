//
// Skyline
// Copyright (C) 2019 Nichole Mattera
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

#include "FooterView.hpp"
#include "../Draw.hpp"
#include "../Application.hpp"

using namespace std;

namespace skyline {
    FooterView::FooterView() : View(SLRectMake(0, FB_HEIGHT - 73, FB_WIDTH, 73)) {}

    FooterView::~FooterView() {}

    void FooterView::render(Rect rect, double dTime) {
        Color lineColor = (Application::sharedApplication->colorSetId == ColorSetId_Light) ? SLColorMake(45, 45, 45, 255) : SLColorMake(255, 255, 255, 255);
        Draw::drawLine(SLPointMake(rect.origin.x + 30, rect.origin.y), SLPointMake(rect.origin.x + rect.size.w - 30, rect.origin.y), lineColor);
    }
}