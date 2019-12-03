

#include "Window.h"
#include <GL/glew.h>

int window_create(Window* w, const char* name, int width, int height, unsigned int flags)
{
    uint32_t window_flags = SDL_WINDOW_OPENGL;

    if (flags & INVISIBLE)
        window_flags |= SDL_WINDOW_HIDDEN;

    if (flags & FULLSCREEN)
        window_flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

    if (flags & BORDERLESS)
        window_flags |= SDL_WINDOW_BORDERLESS;

    w->window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
    w->gl_context = SDL_GL_CreateContext(w->window);

    glewInit();

//    SIRenPrint::print("*** OPENGL VERSION: " + std::string((const char *) glGetString(GL_VERSION)) + " ***");

    glClearColor(0, 0, 1, 1);

    // vertical synchronisation active
    SDL_GL_SetSwapInterval(1);

    //enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    return 0;
}

void window_swap_buffer(Window* w)
{
    SDL_GL_SetSwapInterval(w->window);
}