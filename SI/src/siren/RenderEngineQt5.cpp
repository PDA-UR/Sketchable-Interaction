
#include "RenderEngineQt5.hpp"
#include <siren/window/MainWindow.hpp>
#include <sigrun/log/Log.hpp>

RenderEngineQT5::RenderEngineQT5()
{ SIOBJECT("SIREN")
    Log::SHOW = LOG_SHOW_ALL;
}

RenderEngineQT5::~RenderEngineQT5()
{
    INFO("Terminating Rendering Engine...");
    INFO("Rendering Engine terminated!");
}

void RenderEngineQT5::start(int width, int height, int argc, char **argv)
{
    QApplication qapp(argc, argv);
    MainWindow window;
    window.show();

    qapp.exec();
}

void RenderEngineQT5::run()
{

}

void RenderEngineQT5::pause()
{
    // stop update thread
}
