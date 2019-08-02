

#ifndef SI_CORE_HPP
#define SI_CORE_HPP


#include <core/pysi_core/Scripting.hpp>
#include "core/context/Context.hpp"

#include "render_engine_core/IRenderEngine.hpp"

#ifdef xlib
#include <X11/Xlib.h>
#else
#define DEFAULT_WIDTH 1920
#define DEFAULT_HEIGHT 1080
#endif

class Core
{
public:
    Core();
    ~Core();

    void start(const std::string& plugin_path, IRenderEngine* ire);
    void stop();

    const int width() const;
    const int height() const;
    const std::string& plugin_path() const;

private:
    int d_target_width, d_target_height;
    std::string d_plugin_path;

    Context* p_ctx;
};


#endif //SI_CORE_HPP
