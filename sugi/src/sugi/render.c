#include <stdio.h>
#include "GL/glew.h"
#include "sugi/render.h"
#include "sugi/texture.h"


void sugiRenderTextureId(
        uintptr_t dst_id,
        uint32_t *buffer,
        uint32_t width,
        uint32_t height,
        uint32_t offset_x,
        uint32_t offset_y)
{
    glBindTexture(GL_TEXTURE_2D, dst_id);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, width);
    glTexSubImage2D(GL_TEXTURE_2D, 0, offset_x, offset_y, width, height,
                    GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, buffer);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void sugi_render_texture(
        Texture *texture,
        int32_t x,
        int32_t y,
        int32_t ww,
        int32_t wh)
{
    uintptr_t tex_id = (uintptr_t) texture;
    printf("tex_id: %lu\n", tex_id);
    printf("w: %d\th: %d\n\n", texture->width, texture->height);
    sugiRenderTextureId(tex_id, texture->buffer, texture->width, texture->height, 0, 0);
    glBindTexture(GL_TEXTURE_2D, tex_id);

    // Draw a quad
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_QUADS);
    {
        glTexCoord2d(0.0, 0.0);
        glVertex2d(0.0, 0.0);

        glTexCoord2d(1.0, 0.0);
        glVertex2d(ww, 0.0);

        glTexCoord2d(1.0, 1.0);
        glVertex2d(ww, wh);

        glTexCoord2d(0.0, 1.0);
        glVertex2d(0.0, wh);
    }
    glEnd();

    // Set viewport to center a quad
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, ww, wh, 0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(x, y, ww, wh);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void sugi_render(
        uint32_t *buffer,
        int32_t x,
        int32_t y,
        int32_t rw,
        int32_t rh,
        int32_t ww,
        int32_t wh)
{
    // Create texture
    glPixelStorei(GL_UNPACK_ROW_LENGTH, rw);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, rw, rh,
                    GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, buffer);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

    // Draw a quad
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_QUADS);
    {
        glTexCoord2d(0.0, 0.0);
        glVertex2d(0.0, 0.0);

        glTexCoord2d(1.0, 0.0);
        glVertex2d(ww, 0.0);

        glTexCoord2d(1.0, 1.0);
        glVertex2d(ww, wh);

        glTexCoord2d(0.0, 1.0);
        glVertex2d(0.0, wh);
    }
    glEnd();

    // Set viewport to center a quad
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, ww, wh, 0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(x, y, ww, wh);
}

