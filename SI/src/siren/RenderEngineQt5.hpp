

#ifndef SITEST_RENDERENGINEQT5_HPP
#define SITEST_RENDERENGINEQT5_HPP

#include <SI/SI.hpp>
#include <QApplication>
#include <QMainWindow>
#include <debug/Print.hpp>
#include <siren/window/MainWindow.hpp>

class RenderEngineQT5: public IRenderEngine, public SIObject
{Q_OBJECT
public:
    RenderEngineQT5();
    ~RenderEngineQT5();

    Q_SLOT void start() override;
    Q_SLOT void stop();
    void run() override;
    void pause() override;


private:
    MainWindow* d_window;

protected:
};

#endif //SITEST_RENDERENGINEQT5_HPP
