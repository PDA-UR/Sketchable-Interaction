

#ifndef SI_SIRENDERENGINESDL2_HPP
#define SI_SIRENDERENGINESDL2_HPP

#include "core/render_engine_core/IRenderEngine.hpp"
#include <SDL2/SDL.h>
#include <GL/glew.h>

enum class STATE
{
    ON,
    OFF
};

class SIRenderEngineSDL2 : public IRenderEngine
{
public:
    SIRenderEngineSDL2();
    ~SIRenderEngineSDL2();

    void run() override;
    void start(int width, int height) override;
    void pause() override;

private:
    void draw();

    STATE d_state;
    float d_frame_time;
    int d_width, d_height;

    SDL_Window* p_window = nullptr;
    SDL_GLContext gl_context;
};


#endif //SI_SIRENDERENGINESDL2_HPP
