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

#include <fstream>

#ifndef USE_SDL
    #include <turbojpeg.h>
    #include <png.h>
#endif

#include "Image.hpp"

using namespace std;

namespace skyline {
    Image::Image(string filePath) {
        #ifdef USE_SDL
            SDL_Surface * image = IMG_Load(filePath.c_str());
            SDL_Texture * texture = SDL_CreateTextureFromSurface(Draw::renderer, image);
            SDL_FreeSurface(image);
        #else
            string ext = this->_getExtension(filePath);
            if (ext == "png") {
                this->_loadPng(filePath);
            }
            else if (ext == "jpg" || ext == "jpeg" || ext == "jpe" || ext == "jif" || ext == "jfif" || ext == "jfi") {
                this->_loadJpeg(filePath);
            }
        #endif
    }

    Image::~Image() {
        #ifdef USE_SDL
            if (AssetManager::icon != NULL)
                SDL_DestroyTexture(AssetManager::icon);
        #else
        #endif
    }

    #ifdef USE_SDL
        SDL_Texture * Image::getTexture() {
            return _texture;
        }
    #else
        u8 * Image::getTexture() {
            return _texture;
        }

        Size Image::getImageSize() {
            return _imageSize;
        }
    #endif

    #ifndef USE_SDL
        void Image::_loadPng(std::string filePath) {
            FILE * fp;

            if ((fp = fopen(filePath.c_str(), "rb")) == NULL) {
                return;
            }

            png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
            if (!png) {
                fclose(fp);
                return;
            }

            png_infop info = png_create_info_struct(png);
            if (!info) {
                png_destroy_read_struct(&png, NULL, NULL);
                fclose(fp);
                return;
            }
            
            if (setjmp(png_jmpbuf(png))) {
                png_destroy_read_struct(&png, &info, NULL);
                fclose(fp);
                return;
            }

            png_init_io(png, fp);
            png_read_info(png, info);

            int width = png_get_image_width(png, info);
            int height = png_get_image_height(png, info);
            png_byte color_type = png_get_color_type(png, info);
            png_byte bit_depth = png_get_bit_depth(png, info);

            if(bit_depth == 16)
                png_set_strip_16(png);

            if(color_type == PNG_COLOR_TYPE_PALETTE)
                png_set_palette_to_rgb(png);

            if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
                png_set_expand_gray_1_2_4_to_8(png);

            if(png_get_valid(png, info, PNG_INFO_tRNS))
                png_set_tRNS_to_alpha(png);

            if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_PALETTE)
                png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

            if(color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
                png_set_gray_to_rgb(png);

            png_read_update_info(png, info);

            png_bytep * row_pointers = new png_bytep[sizeof(png_bytep) * height];
            for (s32 y = 0; y < height; y++) {
                row_pointers[y] = new png_byte[png_get_rowbytes(png, info)];
            }

            png_read_image(png, row_pointers);

            s32 pos;
            this->_texture = new u8[width * height * 4];
            for (s32 y = 0; y < height; y++) {
                png_byte * row = row_pointers[y];
                for (s32 x = 0; x < width; x++) {
                    png_byte* ptr = &(row[x * 4]);
                    pos = ((y * width) + x) * 4;

                    this->_texture[pos + 0] = ptr[0];
                    this->_texture[pos + 1] = ptr[1];
                    this->_texture[pos + 2] = ptr[2];
                    this->_texture[pos + 3] = ptr[3];
                }
            }

            _imageSize = SLSizeMake(width, height);

            for (s32 y = 0; y < height; y++) {
                delete[] row_pointers[y];
            }
            delete[] row_pointers;

            png_destroy_read_struct(&png, &info, NULL);
            fclose(fp);
        }

        void Image::_loadJpeg(std::string filePath) {
            ifstream fs(filePath, ifstream::binary);
            if (!fs) {
                return;
            }

            fs.seekg(0, fs.end);
            int size = fs.tellg();
            fs.seekg(0, fs.beg);

            char * buffer = new char[size];
            fs.read(buffer, size);
            fs.close();

            tjhandle _jpegDecompressor = tjInitDecompress();
            if (_jpegDecompressor == NULL) {
                delete[] buffer;
                return;
            }

            int width, height, subSample;
            if (tjDecompressHeader2(_jpegDecompressor, (u8 *) buffer, size, &width, &height, &subSample) == -1) {
                delete[] buffer;
                tjDestroy(_jpegDecompressor);
                return;
            }

            this->_texture = new u8[width * height * 4];
            if (tjDecompress2(_jpegDecompressor, (u8 *) buffer, size, this->_texture, width, 0, height, TJPF_RGBA, TJFLAG_ACCURATEDCT) == -1) {
                delete[] buffer;
                tjDestroy(_jpegDecompressor);
                return;
            }

            _imageSize = SLSizeMake(width, height);

            delete[] buffer;
            tjDestroy(_jpegDecompressor);
        }

        string Image::_getExtension(string filePath) {
            return filePath.substr(filePath.find_last_of(".") + 1);
        }
    #endif
}