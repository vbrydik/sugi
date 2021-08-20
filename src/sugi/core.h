#ifndef CORE_H
#define CORE_H

#include "GL/glew.h"
#include "GLFW/glfw3.h"

// Declare variables
struct SugiCoreStateTag;

typedef struct SugiCoreStateTag {
    void (*init_fn)(void);
    void (*tick_fn)(void);
    void (*draw_fn)(void);
    GLFWwindow *window;
    int quit;
    int dt;
} SugiCoreState;


// Declare methods
void sugi_core_set_init(void (*f)(void));
void sugi_core_set_update(void (*f)(void));
void sugi_core_set_draw(void (*f)(void));
void sugi_call_init_internal(void);
void sugi_call_update_internal(void);
void sugi_call_draw_internal(void);
void sugi_core_run(void);

#endif // CORE_H
