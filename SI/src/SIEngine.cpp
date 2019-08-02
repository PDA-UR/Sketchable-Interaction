
#include <core/Core.hpp>
#include <core/debug/Print.hpp>
#include "SIEngine.hpp"
#include <render_engine/sdl2/SIRenderEngineSDL2.hpp>

Core* SI::p_core = nullptr;
IRenderEngine* SI::s_render_engine = nullptr;

int SI::RENDER_ENGINE_SDL2 = 0;
int SI::RENDER_ENGINE_WINDOW_MANAGER = 1;
int SI::RENDER_ENGINE_CUSTOM = 2;


void SI::start(const char *plugin_path)
{
    p_core = new Core();
    p_core->start(plugin_path, new SIRenderEngineSDL2());
}

void SI::pause()
{

}

void SI::end()
{
    p_core->stop();

    delete p_core;
    p_core = nullptr;
}


int SI_start(const char* plugin_path)
{
    SI::start(plugin_path);

    return 0;
}

int SI_pause()
{
    SI::pause();

    return 0;
}

int SI_end()
{
    SI::end();

    return 0;
}

