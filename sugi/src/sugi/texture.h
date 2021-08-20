/*!
 * \file texture.h
 * \brief this header file contains all defenitions for
 * Texture struct and functions associated with it.
 */
#ifndef TEXTURE_H
#define TEXTURE_H
#include <stdint.h>
#include <stdbool.h>

/*!
 * \brief The Texture struct
 * contains necessary info about texure,
 * like width, height and buffer.
 */
typedef struct Texture {
    uint32_t *buffer;
    uint32_t width;
    uint32_t height;
    uint32_t id;
    bool updated;
} Texture;

/*!
 * \brief sugiTextureInit initializes a Texture struct.
 * \param w width of the Texture.
 * \param h height of the texture.
 * \return initialized Texure struct.
 */
Texture sugiTextureCreate(unsigned int w, unsigned int h);

/*!
 * \brief sugiTextureInitGL initialize Texture to
 * be used as a OpenGL texture.
 * \param texture Texture struct to be initialized.
 */
void sugiTextureInitGL(Texture *texture);

/*!
 * \brief sugiTextureUpdateGL update texture.
 * \param texture Texture to be updated.
 */
void sugiTextureUpdateGL(Texture *texture);

/*!
 * \brief sugiTextureRenderBuffer renders a buffer to
 * a texture with specified size and offset.
 * \param dst destination Texture struct.
 * \param buffer source data buffer.
 * \param width buffer width.
 * \param height buffer height.
 * \param offset_x horizontal offset on the dst Texture.
 * \param offset_y vertical offset on the dst Texture.
 */
void sugiTextureRenderBuffer(
        Texture *dst,
        uint32_t *buffer,
        uint32_t width,
        uint32_t height,
        uint32_t offset_x,
        uint32_t offset_y);

/*!
 * \brief sugiTexturePset put a pixel in a Texture buffer.
 * \param texture destination Texture.
 * \param x X coordinate.
 * \param y Y coordinate.
 * \param c color in 0xRRGGBBAA format.
 */
void sugiTexturePset(Texture *texture, unsigned int x, unsigned int y, unsigned int c);

/*!
 * \brief sugiTextureClear clear or fill Texture buffer with color.
 * \param texture destination Texture.
 * \param c color in 0xRRGGBBAA format.
 */
void sugiTextureClear(Texture *texture, unsigned int c);

#endif // TEXTURE_H
