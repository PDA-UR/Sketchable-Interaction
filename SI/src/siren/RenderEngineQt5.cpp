
#include "RenderEngineQt5.hpp"

#include <QDesktopWidget>
#include <QScreen>

RenderEngineQT5::RenderEngineQT5()
{ SIREN
}

RenderEngineQT5::~RenderEngineQT5()
{
    INFO("Terminating Rendering Engine...");

    INFO("Rendering Engine terminated!");
}

void RenderEngineQT5::start(int& width, int& height)
{
    width = QApplication::primaryScreen()->geometry().width();
    height = QApplication::primaryScreen()->geometry().height();

    d_window = new MainWindow(width, height);

    d_window->setGeometry(QApplication::primaryScreen()->availableGeometry());
    run();
}

void RenderEngineQT5::run()
{
    d_window->showFullScreen();
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
