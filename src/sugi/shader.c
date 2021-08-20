#include "sugi/shader.h"


GLuint sugi_renderer_gl_compile_shader_internal()
{
    GLuint sugi_gl_program = glCreateProgram();

    // Older version! Was used to draw texture on a quad
//    char *vert_shader_src[0x1ffff] = {
//        "\
//        #version 130\n\
//        \
//        void main() {\
//            gl_Position = ftransform();\
//            gl_TexCoord[0] = gl_MultiTexCoord0;\
//        }\
//        "
//    };

//    char *frag_shader_src[0x1ffff] = {
//        "\
//        #version 130\n\
//        \
//        uniform sampler2D text_in;\
//        \
//        void main() {\
//            vec4 c = texture2D(text_in, gl_TexCoord[0].st);\
//            gl_FragColor = c;\
//        }\
//        "
//    };


    char *vert_shader_src[0x1ffff] = {
        "\
        #version 330 core\n\
        \
        in vec2 in_position;\n\
        in vec3 in_color;\n\
        in vec2 in_texcoord;\n\
        \
        out vec3 v_color;\n\
        out vec2 v_texcoord;\n\
        \
        void main() {\
            gl_Position = vec4(in_position, 1.0, 1.0);\
            v_color = in_color;\
            v_texcoord = in_texcoord;\
        }\
        "
    };

    char *frag_shader_src[0x1ffff] = {
        "\
        #version 330\n\
        \
        in vec3 v_color;\n\
        in vec2 v_texcoord;\n\
        \
        out vec4 out_color;\n\
        \
        uniform sampler2D tex;\n\
        \
        void main() {\
            out_color = texture(tex, v_texcoord) * vec4(v_color, 1.0);\
        }\
        "
    };

    // Creating and compiling a Vertex Shader
    GLint vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, vert_shader_src, NULL);
    glCompileShader(vert_shader);
    glAttachShader(sugi_gl_program, vert_shader);

    // Creating and compiling a Fragment Shader
    GLint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, frag_shader_src, NULL);
    glCompileShader(frag_shader);
    glAttachShader(sugi_gl_program, frag_shader);

    // Checking for GL shader compilation errors
    GLint v_compiled = 0;
    glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &v_compiled);
    if (v_compiled == GL_FALSE) {
        printf("ERR: Vertex shader not compiled.\n");
    }
    GLint f_compiled = 0;
    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &f_compiled);
    if (f_compiled == GL_FALSE) {
        printf("ERR: Fragment shader not compiled.\n");
    }
    if (v_compiled == GL_FALSE || f_compiled == GL_FALSE) {
        printf("ERR: Failed to link gl program.\n");
        return 0;
    }
    glLinkProgram(sugi_gl_program);
    return sugi_gl_program;
}

