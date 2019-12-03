

#include <debug/Print.hpp>
#include "RenderEngineSdl2.hpp"
#include <SDL2/SDL.h>
#include <siren/geometry/SpriteBatch.hpp>
#include "display/Window.h"

RenderEngineSDL2::RenderEngineSDL2():
        d_width(800),
        d_height(600),
        d_time(0),
        d_frame_time(0),
        d_state(STATE::ON),
        d_max_fps(60.0)
{

}

RenderEngineSDL2::~RenderEngineSDL2()
{
    SDL_Quit();
}

void RenderEngineSDL2::start(int width, int height)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    Window w;
    window_create(&w, "SI", width, height, SDL_WINDOW_OPENGL);

    d_camera = std::make_unique<Camera2D>();
    d_shader_program = std::make_unique<GLSLProgram>();
    d_rsb = std::make_unique<SpriteBatch>();

    init_shaders();
    d_camera->initialize(d_width, d_height);
    d_rsb->initialize();

    run();
}

void RenderEngineSDL2::run()
{
    d_fps_limiter.begin();
    d_is_running = true;

    d_fps_limiter.end();
}

void RenderEngineSDL2::pause()
{
    Print::print("pause");
}

void RenderEngineSDL2::draw()
{

}

void RenderEngineSDL2::init_shaders()
{
    d_shader_program->compile_shaders("src/siren/res/shaders/color_shading.vert","src/siren/res/shaders/color_shading.frag");
    d_shader_program->add_attribute("vertex_position");
    d_shader_program->add_attribute("vertex_color");
    d_shader_program->link_shaders();
}
