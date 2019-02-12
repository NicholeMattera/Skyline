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
#include <string>

#ifdef USE_SDL
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_image.h>
#else 
    #include "../Types.hpp"
#endif

namespace skyline {
    class Image {
        public:
            Image(std::string imageFile);
            ~Image();
            #ifdef USE_SDL
                SDL_Texture * getTexture();
            #else
                u8 * getTexture();
                Size getImageSize();
            #endif

        private:
            #ifdef USE_SDL
                SDL_Texture * _texture = NULL;
            #else
                u8 * _file = NULL;
                int _fileLength = 0;

                u8 * _texture = NULL;
                Size _imageSize;
                void _loadFile(std::string file);
                void _loadPng(std::string imageFile);
                void _loadJpeg(std::string imageFile);
                std::string _getExtension(std::string imageFile);
            #endif
    };
}