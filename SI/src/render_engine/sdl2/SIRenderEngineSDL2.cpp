

#include <core/debug/Print.hpp>
#include "SIRenderEngineSDL2.hpp"

SIRenderEngineSDL2::SIRenderEngineSDL2()
{
}

SIRenderEngineSDL2::~SIRenderEngineSDL2()
{

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


    run();
}

void SIRenderEngineSDL2::run()
{
    d_is_running = true;

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

    glClearColor(0.0, 0.0, 0.0, 1.0);

    glEnableClientState(GL_COLOR_ARRAY);
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(0, 0);
    glVertex2f(0, 500);
    glVertex2f(500, 500);

    glEnd();

    SDL_GL_SwapWindow(p_window);
}

