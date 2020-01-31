
#include "RenderEngineQt5.hpp"

RenderEngineQT5::RenderEngineQT5()
{ SIREN
}

RenderEngineQT5::~RenderEngineQT5()
{
    INFO("Terminating Rendering Engine...");

    INFO("Rendering Engine terminated!");
}

void RenderEngineQT5::start(int width, int height)
{
    d_window = new MainWindow(width, height);

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

void RenderEngineQT5::stop()
{
    INFO("Stopping Rendering Engine...");
    d_window->close();
    d_window->deleteLater();
    INFO("Rendering Engine stopped!");
}
