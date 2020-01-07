

#ifndef SITEST_RENDERENGINEQT5_HPP
#define SITEST_RENDERENGINEQT5_HPP

#include <SI/SI.hpp>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <debug/Print.hpp>
#include <QTimer>
#include <QElapsedTimer>
#include <QtGui/QtGui>


class RenderEngineQT5: public IRenderEngine, SIObject
{
public:
    RenderEngineQT5();
    ~RenderEngineQT5();

    void start(int width, int height, int argc, char **argv) override;
    void run() override;
    void pause() override;

private:


protected:
};

#endif //SITEST_RENDERENGINEQT5_HPP
