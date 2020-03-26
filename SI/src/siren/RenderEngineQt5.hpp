

#ifndef SITEST_RENDERENGINEQT5_HPP
#define SITEST_RENDERENGINEQT5_HPP

#include <SI/SI.hpp>
#include <QApplication>
#include <QMainWindow>
#include <siren/window/MainWindow.hpp>

class RenderEngineQT5: public IRenderEngine, public SIObject
{Q_OBJECT SIREN
public:
    RenderEngineQT5();
    ~RenderEngineQT5();

    void start(uint32_t& width, uint32_t& height) override;
    void run() override;
    void pause() override;
    void stop() override;

private:
    MainWindow* d_window;

protected:
};

#endif //SITEST_RENDERENGINEQT5_HPP
