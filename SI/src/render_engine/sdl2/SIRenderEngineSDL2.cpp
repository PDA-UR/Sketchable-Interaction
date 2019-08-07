

#include <core/debug/Print.hpp>
#include "SIRenderEngineSDL2.hpp"
#include "render_engine/sdl2/region_sprite/RegionSprite.h"
#include "render_engine/sdl2/shader/GLSLProgram.h"


SIRenderEngineSDL2::SIRenderEngineSDL2() :
    d_width(800),
    d_height(600),
    d_time(0),
    d_frame_time(0),
    p_window(nullptr),
    d_state(STATE::ON)
{
}

SIRenderEngineSDL2::~SIRenderEngineSDL2()
{
    SDL_Quit();
}

void SIRenderEngineSDL2::start(int width, int height)
{
    d_width = width;
    d_height = height;
    d_state = STATE::ON;

    SDL_Init(SDL_INIT_EVERYTHING);
    p_window = SDL_CreateWindow("SI", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, d_width, d_height, SDL_WINDOW_OPENGL);
    gl_context = SDL_GL_CreateContext(p_window);

    glewInit();

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    initialize_shaders();
    run();
}

void SIRenderEngineSDL2::run()
{
    d_is_running = true;

    region_sprite_initialize(&rs, -1, -1, 2, 2);

    while(!is_stop_requested() && d_state == STATE::ON)
    {
        SDL_Event ev;
        while(SDL_PollEvent(&ev))
        {
            switch(ev.type)
            {
                case SDL_QUIT:
                    d_state = STATE::OFF;
                    set_is_running(false);
                    break;

                case SDL_KEYDOWN:
                    if (ev.key.keysym.sym == SDLK_ESCAPE)
                    {
                        d_state = STATE::OFF;
                        set_is_running(false);
                    }

                    break;
            }
        }

        d_time += 0.01;

        draw();
    }

    Print::print("Renderer Terminates");
}

void SIRenderEngineSDL2::pause()
{

}

void SIRenderEngineSDL2::draw()
{
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glslprogram_use(&color_shader_program);

    GLuint time_location = glslprogram_uniform_location(&color_shader_program, "time");

    glUniform1f(time_location, d_time);

    region_sprite_draw(&rs);
    glslprogram_unuse(&color_shader_program);

    SDL_GL_SwapWindow(p_window);
}

void SIRenderEngineSDL2::initialize_shaders()
{
    glslprogram_initialize(&color_shader_program);
    glslprogram_compile_shaders(&color_shader_program, "src/render_engine/sdl2/res/shaders/color_shading.vert", "src/render_engine/sdl2/res/shaders/color_shading.frag");
    glslprogram_add_attribute(&color_shader_program, "vertex_position");
    glslprogram_add_attribute(&color_shader_program, "fragment_color");
    glslprogram_link_shaders(&color_shader_program);
}
