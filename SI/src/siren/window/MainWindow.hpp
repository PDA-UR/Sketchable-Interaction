

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
class RegionRepresentation;
class MainWindow: public SIObject, public QMainWindow
{
public:
    MainWindow(int width, int height);
    MainWindow() = default;

    void set_is_running(bool running);
private:
    bool d_is_exit_requested = false;

    Q_SLOT void loop(double delta, int fps);

    std::map<std::string, std::unique_ptr<RegionRepresentation>> d_region_representations;

    UpdateWorker up_update_worker;

    int d_width, d_height;
};


#endif //SITEST_MAINWINDOW_HPP
