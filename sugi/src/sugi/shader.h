#ifndef SHADER_H
#define SHADER_H

#include <stdio.h>
#include <GL/glew.h>

/*
#define VERT_SHADER "#version 130\nvoid main() { gl_Position = ftransform(); gl_TexCoord[0] = glMultiTexCoord0; }"
#define FRAG_SHADER "#version 130\nuniform sampler2D text_in; void main() { vec4 c = texture2D(text_in, gl_TexCoord[0].st); gl_FragColor = c; }"

const char *vert_shader_src[0x1ffff] = {
    "\
    #version 130\n\
    \
    void main() {\
        gl_Position = ftransform();\
        gl_TexCoord[0] = gl_MultiTexCoord0;\
    }\
    "
};


const char *frag_shader_src[0x1ffff] = {
    "\
    #version 130\n\
    \
    uniform sampler2D text_in;\
    \
    void main() {\
        vec4 c = texture2D(text_in, gl_TexCoord[0].st);\
        gl_FragColor = c;\
    }\
    "
};*/

GLuint sugi_renderer_gl_compile_shader_internal();

#endif // SHADER_H
