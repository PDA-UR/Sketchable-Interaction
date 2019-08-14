
#include <core/Core.hpp>
#include <core/debug/Print.hpp>
#include "SIEngine.hpp"
#include <render_engine/sdl2/SIRenderEngineSDL2.hpp>

namespace SI
{
    Core *SIEngine::p_core = nullptr;
    IRenderEngine *SIEngine::s_render_engine = nullptr;

    int SIEngine::RENDER_ENGINE_SDL2 = 0;
    int SIEngine::RENDER_ENGINE_WINDOW_MANAGER = 1;
    int SIEngine::RENDER_ENGINE_CUSTOM = 2;


    void SIEngine::start(const char *plugin_path)
    {
        p_core = new Core();
        p_core->start(plugin_path, new SIRen::SIRenderEngineSDL2());
    }

    void SIEngine::pause()
    {

    }

    void SIEngine::end()
    {
        p_core->stop();

        delete p_core;
        p_core = nullptr;
    }


    int SI_start(const char *plugin_path)
    {
        SIEngine::start(plugin_path);

        return 0;
    }

    int SI_pause()
    {
        SIEngine::pause();

        return 0;
    }

    int SI_end()
    {
        SIEngine::end();

        return 0;
    }
}
