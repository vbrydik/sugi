#ifndef RENDER_H
#define RENDER_H

#include "sugi/texture.h"

void sugi_render(unsigned int *buffer, int x, int y, int rw, int rh, int ww, int wh);
//void sugi_render(
//        unsigned int *buffer,
//        unsigned int offset_x,
//        unsigned int offset_y,
//        unsigned int subtex_w,
//        unsigned int subtex_h,
//        unsigned int atlas_w);
//void sugi_draw(
//        unsigned int atlas_w,
//        unsigned int atlas_h,
//        unsigned int offset_x,
//        unsigned int offset_y,
//        unsigned int subtex_w,
//        unsigned int subtex_h,
//        unsigned int size_x,
//        unsigned int size_y,
//        int pos_x, int pos_y);

void sugi_render_texture(
        Texture *texture,
        int32_t x,
        int32_t y,
        int32_t ww,
        int32_t wh);

#endif // RENDER_H
