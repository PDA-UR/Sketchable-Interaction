
#ifndef C_OGL_SI_TEST_ENGINE_H
#define C_OGL_SI_TEST_ENGINE_H

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <stdio.h>
#include "rect.h"
#include "shader.h"

typedef struct
{
    SDL_Window* window;
    SDL_GLContext* context;
    int width, height;
    int state;
    Rect r1, r2;
    Shader shader;
    float time;

} Engine;

enum _engine_state
{
    OFF, ON, RUNNING
};

Engine engine_construct(Engine* engine, int width, int height);
void engine_destruct(Engine* engine);

void engine_run(Engine* engine);
void engine_init_systems(Engine* engine);
void engine_init_shaders(Engine* engine);

void engine_process_input(Engine* engine);
void engine_render(Engine* engine);
void engine_loop(Engine* engine);

#endif //C_OGL_SI_TEST_ENGINE_H
