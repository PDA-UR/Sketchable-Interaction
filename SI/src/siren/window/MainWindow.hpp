

#ifndef SITEST_MAINWINDOW_HPP
#define SITEST_MAINWINDOW_HPP

#include <memory>

#include <sigrun/SIObject.hpp>

#include <QMainWindow>
#include <QObject>
#include <QMetaMethod>
#include <QPainter>

#include "../background/UpdateWorker.hpp"
#include "../region/RegionRepresentation.hpp"

class MainWindow: public SIObject, public QMainWindow
{
public:
    MainWindow(int width, int height);
    MainWindow() = default;

    void set_is_running(bool running);
private:
    bool d_is_exit_requested = false;

    void draw_background(QPaintEvent* event);
    void draw_region_representations(QPaintEvent* event);

    Q_SLOT void loop(double delta, int fps);

    std::map<std::string, std::unique_ptr<RegionRepresentation>> d_region_representations;

    UpdateWorker up_update_worker;
    QPainter up_qp;

    int d_width, d_height;

protected:
    void paintEvent(QPaintEvent *event) override;
};


#endif //SITEST_MAINWINDOW_HPP
