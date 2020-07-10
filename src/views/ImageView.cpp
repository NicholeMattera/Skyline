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

#include "ImageView.hpp"
#include "../Draw.hpp"

using namespace std;

namespace skyline {
    ImageView::ImageView(Image * image) : View() {
        _image = image;
    }

    ImageView::~ImageView() {}

    void ImageView::render(Rect rect, double dTime) {
        if (_image != NULL) {
            double w = _image->getImageSize().w;
            double h = _image->getImageSize().h;
            double x = rect.origin.x + (rect.size.w / 2 - w / 2);
            double y = rect.origin.y + (rect.size.h / 2 - h / 2);

            Draw::drawImage(SLRectMake(x, y, w, h), _image->getTexture(), IMAGE_MODE_RGBA32);
        }

        View::render(rect, dTime);
    }
}