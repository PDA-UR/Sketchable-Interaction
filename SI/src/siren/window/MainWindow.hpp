

#ifndef SITEST_MAINWINDOW_HPP
#define SITEST_MAINWINDOW_HPP

#include <memory>

#include <sigrun/SIObject.hpp>

#include <QMainWindow>

#include "../background/UpdateWorker.hpp"
#include "../region/RegionRepresentation.hpp"

class MainWindow: public QMainWindow, SIObject
{
public:
    MainWindow();
    ~MainWindow();

private:
    void loop(double delta, int fps);

    void draw_background(QPaintEvent* event);
    void draw_region_representations(QPaintEvent* event);

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    std::map<std::string, std::unique_ptr<RegionRepresentation>> d_region_representations;

    std::unique_ptr<UpdateWorker> up_update_worker;
    std::unique_ptr<QPainter> up_qp;
};


#endif //SITEST_MAINWINDOW_HPP
