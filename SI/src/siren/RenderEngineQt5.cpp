
#include "RenderEngineQt5.hpp"
#include <sigrun/log/Log.hpp>

RenderEngineQT5::RenderEngineQT5()
{ SIOBJECT(SIREN)
    Log::SHOW = LOG_SHOW_ALL;
}

RenderEngineQT5::~RenderEngineQT5()
{
    INFO("Terminating Rendering Engine...");

    INFO("Rendering Engine terminated!");
}

void RenderEngineQT5::start(int width, int height)
{
    d_window = std::make_unique<MainWindow>(width, height);
    run();
}

void RenderEngineQT5::run()
{
    d_window->show();
}

void RenderEngineQT5::pause()
{
    // stop update thread
}
