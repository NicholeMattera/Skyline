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

#include "Draw.hpp"

namespace skyline {
    void Draw::drawPixel(Point p, Color color) {
        if (p.x >= FB_WIDTH || p.y >= FB_HEIGHT || p.x < 0 || p.y < 0)
            return;

        u32 off = p.y * stride + p.x * 4;
        framebuffer[off] = _blendColor(framebuffer[off], color.r, color.a); off++;
        framebuffer[off] = _blendColor(framebuffer[off], color.g, color.a); off++;
        framebuffer[off] = _blendColor(framebuffer[off], color.b, color.a); off++;
        framebuffer[off] = 0xFF;
    }

    void Draw::drawLine(Point p1, Point p2, Color color) {
        if (p1.x == p2.x && p1.y == p2.y) {
            drawPixel(p1, color);
        }
        else if (p1.y == p2.y) {
            _drawHorizontalLine(p1, p2, color);
        }
        else if (p1.x == p2.x) {
            _drawVerticalLine(p1, p2, color);
        }
        else {
            _drawSlopedLine(p1, p2, color);
        }
    }

    void Draw::drawFilledRectangle(Rect rect, Color color) {
        for (s16 j = rect.origin.y; j < rect.origin.y + rect.size.h; j++) {
            for (s16 i = rect.origin.x; i < rect.origin.x + rect.size.w; i++) {
                drawPixel(SLPointMake(i, j), color);
            }
        }
    }

    // Private Methods

    u8 Draw::_blendColor(u8 src, u8 dst, u8 alpha) {
        u8 one_minus_alpha = static_cast<u8>(255) - alpha;
        return (dst * alpha + src * one_minus_alpha) / static_cast<u8>(255);
    }

    void Draw::_drawHorizontalLine(Point p1, Point p2, Color color) {
        Point lowerP = (p1.x < p2.x) ? p1 : p2;
        Point higherP = (p1.x > p2.x) ? p1 : p2;

        for (s16 x = lowerP.x; x <= higherP.x; x++) {
            drawPixel(SLPointMake(x, p1.y), color);
        }
    }

    void Draw::_drawVerticalLine(Point p1, Point p2, Color color) {
        Point lowerP = (p1.y < p2.y) ? p1 : p2;
        Point higherP = (p1.y > p2.y) ? p1 : p2;

        for (s16 y = lowerP.y; y <= higherP.y; y++) {
            drawPixel(SLPointMake(p1.x, y), color);
        }
    }

    // Bresenham's algorithm
    void Draw::_drawSlopedLine(Point p1, Point p2, Color color) {
        Point lowerP = (p1.x < p2.x) ? p1 : p2;
        Point higherP = (p1.x > p2.x) ? p1 : p2;

        s16 dx = higherP.x - lowerP.x;
        s16 dy = higherP.y - lowerP.y;
        s16 d = 2 * dy - dx;
        s16 incrE = 2 * dy;
        s16 incrNE = 2 * (dy - dx);
        s16 x = lowerP.x;
        s16 y = lowerP.y;

        drawPixel(lowerP, color);

        while(x < higherP.x) {
            if (d <= 0) {
                d += incrE;
                x++;
            } else {
                d += incrNE;
                x++;
                y++;
            }

            drawPixel(SLPointMake(x, y), color);
        }
    }
}
