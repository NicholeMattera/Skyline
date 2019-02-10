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
    void Draw::drawInit() {
        #ifdef USE_SDL
            SDL_Init(SDL_INIT_VIDEO);

            _window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_FULLSCREEN);
            if (_window) {
                Draw::renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            }
        #else
            _window = nwindowGetDefault();
            
            framebufferCreate(&Draw::framebuffer, _window, FB_WIDTH, FB_HEIGHT, PIXEL_FORMAT_RGBA_8888, 2);
            framebufferMakeLinear(&Draw::framebuffer);
        #endif
    }

    void Draw::drawExit() {
        #ifdef USE_SDL
            if (Draw::renderer != NULL)
                SDL_DestroyRenderer(Draw::renderer);
    
            if (_window != NULL)
                SDL_DestroyWindow(_window);

            SDL_Quit();
        #else
            framebufferClose(&Draw::framebuffer);
        #endif
    }

    void Draw::drawPixel(Point p, Color color) {
        #ifdef USE_SDL
            // TODO
        #else
            if (p.x >= FB_WIDTH || p.y >= FB_HEIGHT || p.x < 0 || p.y < 0)
                return;

            u32 off = p.y * Draw::stride + p.x * 4;
            Draw::currentFramebuffer[off] = _blendColor(Draw::currentFramebuffer[off], color.r, color.a); off++;
            Draw::currentFramebuffer[off] = _blendColor(Draw::currentFramebuffer[off], color.g, color.a); off++;
            Draw::currentFramebuffer[off] = _blendColor(Draw::currentFramebuffer[off], color.b, color.a); off++;
            Draw::currentFramebuffer[off] = 0xFF;
        #endif
    }

    void Draw::drawLine(Point p1, Point p2, Color color) {
        #ifdef USE_SDL
            // TODO
        #else
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
        #endif
    }

    void Draw::drawFilledRectangle(Rect rect, Color color) {
        #ifdef USE_SDL
            // TODO
        #else
            for (s16 j = rect.origin.y; j < rect.origin.y + rect.size.h; j++) {
                for (s16 i = rect.origin.x; i < rect.origin.x + rect.size.w; i++) {
                    drawPixel(SLPointMake(i, j), color);
                }
            }
        #endif
    }

    void Draw::drawImage(Rect rect, const u8 * image, ImageMode mode) {
        #ifdef USE_SDL
            // TODO
        #else
            s32 tmpx, tmpy;
            s32 pos;
            Color current_color;

            for (tmpy = 0; tmpy < rect.size.h; tmpy++) {
                for (tmpx = 0; tmpx < rect.size.w; tmpx++) {
                    switch (mode) {
                        case IMAGE_MODE_RGB24:
                            pos = ((tmpy * rect.size.w) + tmpx) * 3;
                            current_color = SLColorMake(image[pos + 0], image[pos + 1], image[pos + 2], 255);
                            break;
                        case IMAGE_MODE_RGBA32:
                            pos = ((tmpy * rect.size.w) + tmpx) * 4;
                            current_color = SLColorMake(image[pos + 0], image[pos + 1], image[pos + 2], image[pos + 3]);
                            break;
                        case IMAGE_MODE_BGR24:
                            pos = ((tmpy * rect.size.w) + tmpx) * 3;
                            current_color = SLColorMake(image[pos + 2], image[pos + 1], image[pos + 0], 255);
                            break;
                        case IMAGE_MODE_ABGR32:
                            pos = ((tmpy * rect.size.w) + tmpx) * 4;
                            current_color = SLColorMake(image[pos + 3], image[pos + 2], image[pos + 1], image[pos + 0]);
                            break;
                    }

                    drawPixel(SLPointMake(rect.origin.x + tmpx, rect.origin.y + tmpy), current_color);
                }
            }
        #endif
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
