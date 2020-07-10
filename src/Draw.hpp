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
#include "Types.hpp"

#define FB_WIDTH 1280
#define FB_HEIGHT 720

namespace skyline {
    class Draw {
        public:
            static void drawInit();
            static void drawExit();

            static void drawPixel(Point p, Color color);
            static void drawLine(Point p1, Point p2, Color color);
            static void drawFilledRectangle(Rect rect, Color color);
            static void drawImage(Rect rect, const u8 * image, ImageMode imageMode);
            
            #ifdef USE_SDL
                static inline SDL_Renderer * renderer;
            #else
                static inline Framebuffer framebuffer;
                static inline u8 * currentFramebuffer;
                static inline u32 stride;
            #endif
        private:
            static u8 _blendColor(u8 src, u8 dst, u8 alpha);
            static void _drawHorizontalLine(Point p1, Point p2, Color color);
            static void _drawVerticalLine(Point p1, Point p2, Color color);
            static void _drawSlopedLine(Point p1, Point p2, Color color);

            #ifdef USE_SDL
                static inline SDL_Window * _window;
            #else
                static inline NWindow * _window;
            #endif
    };
}
