#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <GL/glew.h>
#include "sugi/core.h"
#include "sugi/config.h"
#include "sugi/render.h"
#include "sugi/shader.h"
#include "sugi/texture.h"
#include "array/array.h"


SugiCoreState m_core_state;

void sugi_core_set_init(void (*f)(void))   { m_core_state.init_fn = f; }
void sugi_core_set_update(void (*f)(void)) { m_core_state.tick_fn = f; }
void sugi_core_set_draw(void (*f)(void))   { m_core_state.draw_fn = f; }
void sugi_call_init_internal(void)   { if (m_core_state.init_fn) m_core_state.init_fn(); }
void sugi_call_update_internal(void) { if (m_core_state.tick_fn) m_core_state.tick_fn(); }
void sugi_call_draw_internal(void)   { if (m_core_state.draw_fn) m_core_state.draw_fn(); }
void glfw_error(int code, const char *desc) { printf("GLFW ERROR - [%d]\t%s\n", code, desc); }


void sugi_core_run(void)
{
    SugiCoreState *state = &m_core_state;
    state->quit = 0;
    state->dt = 0;

    if (!glfwInit())
    { 
        // TODO: replace with some proper logging system
        printf("[ERROR] - core - Could not initialize GLFW");
        return;
    }

    glfwSetErrorCallback(glfw_error);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    state->window = glfwCreateWindow(SUGI_CONF_WINDOW_WIDTH, SUGI_CONF_WINDOW_HEIGHT, "sugi", NULL, NULL);
    if (!state->window)
    {
        glfwTerminate();
        printf("[ERROR] - core - Could not create GLFW window");
        return;
    }

    glfwMakeContextCurrent(state->window);
    glfwSwapInterval(0);
    glewInit();

    // Compile a simple shader and use it
    GLuint program = sugi_renderer_gl_compile_shader_internal();

    // Create atlas to store all textures on
    Texture atlas;
    atlas = sugiTextureCreate(128, 128);

    Texture texture;
    texture = sugiTextureCreate(32, 32);
    sugiTextureClear(&texture, 0x22222222);
    sugiTexturePset(&texture, 0, 0, 0xff0000ff);
    sugiTexturePset(&texture, 1, 0, 0x00ff00ff);
    sugiTexturePset(&texture, 2, 0, 0x0000ffff);
    sugiTexturePset(&texture, 3, 0, 0xffffffff);
    sugiTexturePset(&texture, 0, 1, 0xff00ffff);
    sugiTexturePset(&texture, 1, 1, 0x00ffffff);
    sugiTexturePset(&texture, 2, 1, 0xff00ffff);
    sugiTexturePset(&texture, 3, 1, 0xffffffff);

    // Render texture to atlas for testing
    sugiTextureRenderBuffer(&atlas, texture.buffer, texture.width, texture.height, 16, 16);
    sugi_call_init_internal();

    // VAO - Vertex Array Object
    GLuint vao;
    {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // VBO - Vertex Buffer Object
        GLuint vbo;
        glGenBuffers(1, &vbo); // Create one vertex buffer
        // make vertex buffer active (bind it)
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        // Vertices
        float vertices[] = {
        //  Position    Color          Texcoords
            -0.5,  0.5, 1.0, 1.0, 1.0, 0.0, 0.0, // top left
             0.5,  0.5, 1.0, 1.0, 1.0, 1.0, 0.0, // top right
             0.5, -0.5, 1.0, 1.0, 1.0, 1.0, 1.0, // bottom right
            -0.5, -0.5, 1.0, 1.0, 1.0, 0.0, 1.0, // bottom left
        };

        // copy vertex data to vertex buffer or vbo
        // GL_STATIC_DRAW - uploaded once, drawn many times.
        // GL_DYNAMIC_DRAW - uploaded once, can be changed, drawn many times.
        // GL_STREAM_DRAW - uploaded once, drawn once (fast write, slower draw)
        glBufferData(GL_ARRAY_BUFFER, sizeof (vertices), vertices, GL_STATIC_DRAW);

        GLuint indicies[] = {
            0, 1, 2,
            2, 3, 0
        };

        // create one indicies or elements buffer
        GLuint ibo;
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof (indicies), indicies, GL_STATIC_DRAW);

        // Compile and use program
        glUseProgram(program);

        // Bind and generate texture
        sugiTextureInitGL(&atlas);

        // make link between vertex data and attributes
        // Get position of 'position' attribute in shader program
        GLint posAttrib = glGetAttribLocation(program, "in_position");
        glEnableVertexAttribArray(posAttrib);
        GLint colAttrib = glGetAttribLocation(program, "in_color");
        glEnableVertexAttribArray(colAttrib);
        GLint texAttrib = glGetAttribLocation(program, "in_texcoord");
        glEnableVertexAttribArray(texAttrib);

        // Specify how data in a vertex buffer is layed out and later retrieved
        // 0: input reference (attribute location)
        // 1: number of values for that input
        // 2: type of each component
        // 3: should be normalized? (-1.0 to 1.0 or 0.0 to 1.0)
        // 4: stride - how many bytes are between each 'position' attribure
        // 5: offset - how many bytes from the start the attribute 'position' occurs
        glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE,
                  7*sizeof (float), 0);
        glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE,
                  7*sizeof (float), (void*)(2*sizeof (float)));
        glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE,
                  7*sizeof (float), (void*)(5*sizeof (float)));

        // GLuint tex;
        // glGenTextures(1, &tex);
        // glBindTexture(GL_TEXTURE_2D, tex);
        // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture.width, texture.height,
        //              0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, texture.buffer);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

        glBindTexture(GL_TEXTURE_2D, -1);
        glBindVertexArray(-1);
        glUseProgram(-1);
        glBindBuffer(GL_ARRAY_BUFFER, -1);
    }

    sugiTexturePset(&atlas, 0, 2, 0xff0000ff);
    sugiTexturePset(&atlas, 1, 2, 0xffff00ff);
    sugiTexturePset(&atlas, 2, 2, 0x0000ffff);
    sugiTexturePset(&atlas, 3, 2, 0xff00ffff);
    sugiTextureUpdateGL(&atlas);

    // Get position of 'triangleColor' attribute in shader program
    // GLint triColor = glGetUniformLocation(program, "u_color");
    // glUniform3f(triColor, 1.0, 0.0, 0.0);


    while (!glfwWindowShouldClose(state->window) || state->quit)
    {
        // glUniform3f(triColor, red, 0.0, 0.0);
        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glEnable(GL_BLEND);

        // sugi_render_texture(&texture, 16, 16, 64, 64);
        // sugi_render_texture(&atlas, 96, 16, 256, 256);

        // Bind what object or VAO to draw
        // this will load all the states, like VBO, shaders, etc that were bound to it
        glBindVertexArray(vao);
        // 0: what primitive to draw
        // 1: how many vertices to skip
        // 2: how many vertices to draw
        // draw verticies directly
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        // draw elements/indicies with respective verticies
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(-1);

        glfwSwapBuffers(state->window);
        glfwPollEvents();

        sugi_call_update_internal();
        sugi_call_draw_internal();
    }

    // free(atlas.buffer);
    // free(texture.buffer);
    glfwTerminate();
    return;
}

