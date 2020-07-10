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

#pragma once

#include <switch.h>

#define SLPointMake(x, y) (Point) { (s16) (x), (s16) (y) }
#define SLSizeMake(w, h) (Size) { (s16) (w), (s16) (h) }
#define SLRectMake(x, y, w, h) (Rect) { { (s16) (x), (s16) (y) }, { (s16) (w), (s16) (h) } }
#define SLColorMake(r, g, b, a) (Color) { (u8) (r), (u8) (g), (u8) (b), (u8) (a) }

#define SLRectContainsPoint(rect, point) ( \
    point.x >= rect.origin.x && \
    point.x <= rect.origin.x + rect.size.w && \
    point.y >= rect.origin.y && \
    point.y <= rect.origin.y + rect.size.h \
)

namespace skyline {
    typedef enum {
        IMAGE_MODE_RGB24,
        IMAGE_MODE_RGBA32,
        IMAGE_MODE_BGR24,
        IMAGE_MODE_ABGR32
    } ImageMode;

    struct Point {
        s16 x;
        s16 y;
    };

    struct Size {
        s16 w;
        s16 h;
    };

    struct Rect {
        Point origin;
        Size size;
    };

    struct Color {
        u8 r;
        u8 g;
        u8 b;
        u8 a;
    };
}
