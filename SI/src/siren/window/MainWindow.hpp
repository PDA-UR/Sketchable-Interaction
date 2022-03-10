

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
#include <QQmlContext>
#include "../region/RegionRepresentation.hpp"
#include "../region/PartialRegionRepresentation.hpp"


class MainWindow: public QGraphicsView, public SIObject
{ Q_OBJECT


SIREN
public:
    MainWindow(uint32_t width, uint32_t height, uint32_t target_fps);

    void pause();
    bool d_is_running = false;
    void loop();
    void set_cursor_stroke_color_by_cursor_id(const std::string &cursor_id, const glm::vec4 &color);
    void set_cursor_stroke_width_by_cursor_id(const std::string &cursor_id, int stroke_width);

    QQmlEngine* engine();

private:
    void __loop();

    void handle_region_representations();
    void handle_partial_region_representations();

    std::vector<RegionRepresentation*> d_reg_reps;
    std::vector<PartialRegionRepresentation*> d_par_reg_reps;

    std::unordered_map<std::string, std::tuple<int, glm::vec4>> d_cursor_stroke_data;

    uint8_t d_drawing_pen_color = 0;
    uint32_t d_width, d_height, d_target_fps;

    QQmlEngine* d_engine;
    QQmlContext*  d_qmlcontext;

    QGraphicsScene* p_scene;
};


#endif //SITEST_MAINWINDOW_HPP
