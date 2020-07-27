
#include "RenderEngineQt5.hpp"

#include <QDesktopWidget>
#include <QScreen>

RenderEngineQT5::RenderEngineQT5() = default;

RenderEngineQT5::~RenderEngineQT5()
{
    INFO("Terminating Rendering Engine...");

    INFO("Rendering Engine terminated!");
}

void RenderEngineQT5::start(uint32_t width, uint32_t height, uint32_t target_fps)
{
    d_window = new MainWindow(width, height, target_fps);

    d_window->setGeometry(QApplication::primaryScreen()->availableGeometry());

    run();
}

void RenderEngineQT5::run()
{
    d_window->showFullScreen();
    d_window->loop();
}

void RenderEngineQT5::pause()
{
    d_window->pause();
}

void RenderEngineQT5::stop()
{
    INFO("Stopping Rendering Engine...");
    d_window->d_is_running = false;
    INFO("Rendering Engine stopped!");
}
