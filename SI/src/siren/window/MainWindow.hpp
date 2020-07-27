

#ifndef SITEST_MAINWINDOW_HPP
#define SITEST_MAINWINDOW_HPP

#include <memory>
#include <queue>
#include <SI/SI.hpp>
#include <QMainWindow>
#include <QGraphicsAnchorLayout>
#include <QObject>
#include <QQmlEngine>
#include <QMetaMethod>
#include <QPainter>
#include "../region/RegionRepresentation.hpp"
#include <QtCore/QTime>

//class RegionRepresentation;
class MainWindow: public SIObject, public QMainWindow
{ SIREN
public:
    MainWindow(uint32_t width, uint32_t height, uint32_t target_fps);
    MainWindow() = default;

    void pause();
    bool d_is_running = false;
    void loop();

private:
    void __loop();

    std::vector<RegionRepresentation*> d_reg_reps;

    uint32_t d_width, d_height, d_target_fps;

    QQmlEngine* engine;
};


#endif //SITEST_MAINWINDOW_HPP
