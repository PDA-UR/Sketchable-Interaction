
#include "RenderEngineQt5.hpp"

//#include <QDesktopWidget>
#include <QScreen>
#include <debug/Print.hpp>

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

void RenderEngineQT5::disable_anti_aliasing()
{
    QSurfaceFormat format;
    format.setSamples(0);
    QSurfaceFormat::setDefaultFormat(format);
}

void RenderEngineQT5::enable_anti_aliasing(uint32_t samplng_factor)
{
    samplng_factor = 2;
    QSurfaceFormat format;
    format.setSamples(samplng_factor);
    format.setSwapInterval(0);

    QSurfaceFormat::setDefaultFormat(format);
}

void RenderEngineQT5::set_cursor_stroke_width_by_cursor_id(const std::string &cursor_id, int stroke_width)
{
    d_window->set_cursor_stroke_width_by_cursor_id(cursor_id, stroke_width);
}

void RenderEngineQT5::set_cursor_stroke_color_by_cursor_id(const std::string &cursor_id, const glm::vec4 &color)
{
    d_window->set_cursor_stroke_color_by_cursor_id(cursor_id, color);
}
