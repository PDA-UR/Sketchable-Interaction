
#include "engine.h"

Engine engine_construct(Engine* engine, int width, int height)
{
    engine->width = width;
    engine->height = height;
    engine->time = 0.0f;
    engine->state = ON;
}

void engine_destruct(Engine* engine)
{

}

void engine_run(Engine* engine)
{
    engine_init_systems(engine);

    engine->r1 = rect_construct(-1.0f, -1.0f, 1.0f, 1.0f);
    engine->r2 = rect_construct(0.0f, 0.0f, 1.0f, 1.0f);

    engine_loop(engine);
}

void engine_init_systems(Engine* engine)
{
    SDL_Init(SDL_INIT_EVERYTHING);

    engine->window = SDL_CreateWindow(__FILE__, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, engine->width, engine->height, SDL_WINDOW_OPENGL);
    engine->context = SDL_GL_CreateContext(engine->window);

    glewExperimental = GL_TRUE;
    glewInit();

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    glClearColor(0.0, 0.0, 1.0, 1.0);

    engine_init_shaders(engine);
}

void engine_init_shaders(Engine* engine)
{
    engine->shader = shader_construct();

    shader_compile(&(engine->shader), "../shader/color_shading.vert", "../shader/color_shading.frag");
    shader_add_attribute(&engine->shader, "vertex_position");
    shader_add_attribute(&engine->shader, "vertex_color");
    shader_link(&engine->shader);
}

void engine_process_input(Engine* engine)
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
            {
                engine->state = OFF;
            }
            break;

            case SDL_MOUSEMOTION:
            {
                printf("Mouse: (%d, %d)\n", event.motion.x, event.motion.y);
            }
            break;
        }
    }
}

void engine_render(Engine* engine)
{
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnableClientState(GL_COLOR_ARRAY);

    shader_use(&engine->shader);

    GLuint time_location = shader_get_uniform_location(&engine->shader, "time");

    glUniform1f(time_location, engine->time);

    rect_render(&engine->r1);
    rect_render(&engine->r2);

    shader_unuse(&engine->shader);

    SDL_GL_SwapWindow(engine->window);
}

void engine_loop(Engine* engine)
{
    while(engine->state == ON)
    {
        engine_process_input(engine);
        engine->time += 0.01f;
        engine_render(engine);
    }
}