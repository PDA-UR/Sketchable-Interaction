

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
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QOpenGLWidget>
#include "../region/RegionRepresentation.hpp"
#include "../region/PartialRegionRepresentation.hpp"


class MainWindow: public QGraphicsView, public SIObject
{ Q_OBJECT SIREN
public:
    MainWindow(uint32_t width, uint32_t height, uint32_t target_fps);

    void pause();
    bool d_is_running = false;
    void loop();

    QQmlEngine* engine();

private:
    void __loop();

    void handle_region_representations();
    void handle_partial_region_representations();

    std::vector<RegionRepresentation*> d_reg_reps;
    std::vector<PartialRegionRepresentation*> d_par_reg_reps;

    uint32_t d_width, d_height, d_target_fps;

    QQmlEngine* d_engine;

    QGraphicsScene* p_scene;

};


#endif //SITEST_MAINWINDOW_HPP
