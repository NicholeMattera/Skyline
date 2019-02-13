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
    Image::Image(string imageFile) {
        #ifdef USE_SDL
            SDL_Surface * image = IMG_Load(imageFile.c_str());
            SDL_Texture * texture = SDL_CreateTextureFromSurface(Draw::renderer, image);
            SDL_FreeSurface(image);
        #else
            string ext = this->_getExtension(imageFile);
            if (ext == "png") {
                this->_loadPng(imageFile);
            }
            else if (ext == "jpg" || ext == "jpeg" || ext == "jpe" || ext == "jif" || ext == "jfif" || ext == "jfi") {
                this->_loadJpeg(imageFile);
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
    #endif

    #ifndef USE_SDL
        void Image::_loadPng(std::string imageFile) {
            FILE * fp;

            if ((fp = fopen(imageFile.c_str(), "rb")) == NULL) {
                return;
            }

            png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
            if (!png_ptr) {
                fclose(fp);
                return;
            }

            png_infop info_ptr = png_create_info_struct(png_ptr);
            if (!info_ptr) {
                png_destroy_read_struct(&png_ptr, NULL, NULL);
                fclose(fp);
                return;
            }
            
            if (setjmp(png_jmpbuf(png_ptr))) {
                png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
                fclose(fp);
                return;
            }

            png_init_io(png_ptr, fp);
            png_set_sig_bytes(png_ptr, 8);

            png_read_info(png_ptr, info_ptr);

            int width = png_get_image_width(png_ptr, info_ptr);
            int height = png_get_image_height(png_ptr, info_ptr);
            png_byte color_type = png_get_color_type(png_ptr, info_ptr);

            if (color_type != PNG_COLOR_TYPE_RGB && color_type != PNG_COLOR_TYPE_RGBA) {
                png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
                fclose(fp);
                return;
            }

            png_set_interlace_handling(png_ptr);
            png_read_update_info(png_ptr, info_ptr);
            
            if (setjmp(png_jmpbuf(png_ptr))) {
                png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
                fclose(fp);
                return;
            }

            png_bytep * row_pointers = new png_bytep[sizeof(png_bytep) * height];
            for (s32 y = 0; y < height; y++) {
                row_pointers[y] = new png_byte[png_get_rowbytes(png_ptr, info_ptr)];
            }

            png_read_image(png_ptr, row_pointers);

            s32 pos;
            this->_texture = new u8[width * height * 4];
            for (s32 y = 0; y < height; y++) {
                png_byte * row = row_pointers[y];
                for (s32 x = 0; x < width; x++) {
                    if (color_type == PNG_COLOR_TYPE_RGB) {
                        png_byte* ptr = &(row[x * 3]);
                        pos = ((y * width) + x) * 3;

                        this->_texture[pos + 0] = ptr[0];
                        this->_texture[pos + 1] = ptr[1];
                        this->_texture[pos + 2] = ptr[2];
                        this->_texture[pos + 3] = 0xFF;
                    } else if (color_type == PNG_COLOR_TYPE_RGBA) {
                        png_byte* ptr = &(row[x * 4]);
                        pos = ((y * width) + x) * 4;

                        this->_texture[pos + 0] = ptr[0];
                        this->_texture[pos + 1] = ptr[1];
                        this->_texture[pos + 2] = ptr[2];
                        this->_texture[pos + 3] = ptr[3];
                    }
                }
            }

            _imageSize = SLSizeMake(width, height);

            for (s32 y = 0; y < height; y++) {
                delete[] row_pointers[y];
            }
            delete[] row_pointers;

            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            fclose(fp);
        }

        void Image::_loadJpeg(std::string imageFile) {
            ifstream fs(imageFile, ifstream::binary);
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

        string Image::_getExtension(string imageFile) {
            return imageFile.substr(imageFile.find_last_of(".") + 1);
        }
    #endif
}