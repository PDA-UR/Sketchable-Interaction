
#include "RenderEngineQt5.hpp"
#include <sigrun/log/Log.hpp>

RenderEngineQT5::RenderEngineQT5()
{ SIOBJECT("SIREN")
    Log::SHOW = LOG_SHOW_ALL;
}

RenderEngineQT5::~RenderEngineQT5()
{
    INFO("Terminating Rendering Engine...");
    if(d_window)
    {
        INFO("Destroying Window...");

        delete d_window;
        d_window = nullptr;
    }

    INFO("Rendering Engine terminated!");
}

void RenderEngineQT5::start()
{
    d_window = new MainWindow();
    run();
}

void RenderEngineQT5::stop()
{

}

void RenderEngineQT5::run()
{
    d_window->show();
}

void RenderEngineQT5::pause()
{
    // stop update thread
}
