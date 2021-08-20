#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <GL/glew.h>
#include "sugi/texture.h"


Texture sugiTextureCreate(uint32_t w, uint32_t h)
{
    Texture texture = {
        .buffer = (uint32_t *) malloc(sizeof (uint32_t) * w * h),
        .width = w,
        .height = h,
        .updated = false,
    };
    return texture;
}


void sugiTextureInitGL(Texture *texture)
{
    glGenTextures(1, &texture->id);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture->width, texture->height,
                 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, texture->buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    texture->updated = true;
}

void sugiTextureUpdateGL(Texture *texture)
{
    if (texture->updated) {
        glBindTexture(GL_TEXTURE_2D, texture->id);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, texture->width);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, texture->width, texture->height,
                GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, texture->buffer);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        texture->updated = false;
    }
}

void sugiTextureBindGL(Texture *texture)
{
    glBindTexture(GL_TEXTURE_2D, texture->id);
}


void sugiTextureRenderBuffer(
        Texture *texture,
        uint32_t *buffer,
        uint32_t width,
        uint32_t height,
        uint32_t offset_x,
        uint32_t offset_y)
{
    uint32_t dst_y_offset, src_y_offset;
    for (uint32_t i=0; i<height; i++) {
        src_y_offset = i * width;
        dst_y_offset = offset_x + i * texture->width + offset_y * texture->width;
        memcpy(texture->buffer + dst_y_offset, buffer + src_y_offset, sizeof (uint32_t) * width);
    }
    texture->updated = true;
}


void sugiTexturePset(Texture *texture, uint32_t x, uint32_t y, uint32_t c)
{
    *(texture->buffer + x + y * texture->width) = c;
    texture->updated = true;
}


uint32_t sugiTexturePget(Texture *texture, uint32_t x, uint32_t y)
{
    return *(texture->buffer + x + y * texture->width);
}


void sugiTextureClear(Texture *texture, uint32_t c)
{
    memset(texture->buffer, c, sizeof (uint32_t) * texture->width * texture->height);
    texture->updated = true;
}
