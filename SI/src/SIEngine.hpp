
#ifndef SI_SIENGINE_HPP
#define SI_SIENGINE_HPP

#include <core/Core.hpp>
#include <core/render_engine_core/IRenderEngine.hpp>

#define xlib 1

#ifdef __cplusplus
namespace SI
{
    class SIEngine
    {
    public:
        static void start(const char *plugin_path);

        static void pause();

        static void end();

        static int RENDER_ENGINE_SDL2;
        static int RENDER_ENGINE_WINDOW_MANAGER;
        static int RENDER_ENGINE_CUSTOM;

    private:
        static Core *p_core;
        static int s_render_engine_id;
        static IRenderEngine *s_render_engine;
    };

    extern "C"
    {
    int SI_start(const char *plugin_path);
    int SI_pause();
    int SI_end();
    }
}
#endif


#endif //SI_SIENGINE_HPP
