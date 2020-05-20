

#ifndef SITEST_MAINWINDOW_HPP
#define SITEST_MAINWINDOW_HPP

#include <memory>
#include <SI/SI.hpp>
#include <QMainWindow>
#include <QGraphicsAnchorLayout>
#include <QObject>
#include <QMetaMethod>
#include <QPainter>
#include "../background/UpdateWorker.hpp"
#include "../region/RegionRepresentation.hpp"

//class RegionRepresentation;
class MainWindow: public SIObject, public QMainWindow
{ SIREN
public:
    MainWindow(uint32_t width, uint32_t height);
    MainWindow() = default;

private:
    Q_SLOT void loop(double delta, uint32_t fps);

    std::vector<RegionRepresentation*> d_reg_reps;

    UpdateWorker up_update_worker;

    uint32_t d_width, d_height;
};


#endif //SITEST_MAINWINDOW_HPP
