//
// Created by juergen on 22/01/19.
//

#include <QtGui/QPainter>
#include <QKeyEvent>
#include <QPaintEvent>
#include "engine.h"
#include "../si_debug/debug.h"
#include "signal_handler.h"
#include "../si_stdlib/canvas_region.h"
#include "../si_stdlib/mouse_region.h"
#include <QMouseEvent>
#include <QBitmap>
#include <QtWidgets/QGridLayout>
#include <QMouseEvent>
#include <QEvent>

namespace si
{
    /* Engine */

    /* PUBLIC */
    Engine * Engine::s_instance = nullptr;

    Engine *Engine::__instance__()
    {
        static CGuard guard;

        if(!s_instance)
            s_instance = new Engine();

        return s_instance;
    }

    void Engine::start()
    {
        setMouseTracking(true);
        setWindowState(Qt::WindowFullScreen);

        connect(p_step, &step::on_step, this, &Engine::on_step);
        p_step->start();

        main_canvas_region->setObjectName("main_canvas");

        setCentralWidget(main_canvas_region);

        d_active_regions.emplace_back(new neutral(main_canvas_region));

        show();
    }

    void Engine::stop()
    {
        close();
    }

    void Engine::add_region_template(region *r)
    {
        r->on_enter(1);
        r->on_continuous(1);
        r->on_leave(1);
    }

    /* PRIVATE */

    Engine::Engine() : QMainWindow(), p_step(new step(33.0)), main_canvas_region(new canvas(QPolygon(QVector<QPoint>({QPoint(0, 0), QPoint(0, 1080), QPoint(1920, 1080), QPoint(1920, 0)})), this))
    {
        signal(SIGINT, signal_handler::handle);
    }

    Engine::Engine(const Engine &) : QMainWindow(), p_step(new step(33.0)), main_canvas_region(new canvas(QPolygon(QVector<QPoint>({QPoint(0, 0), QPoint(0, 1080), QPoint(1920, 1080), QPoint(1920, 0)})), this))
    {
        signal(SIGINT, signal_handler::handle);
    }

    Engine::~Engine()
    {
        p_step->stop();

        while (!p_step->isFinished());

        delete p_step;
        p_step = nullptr;

        d_active_regions.resize(0); // get rid of all pointers to active regions

        main_canvas_region->close();
        delete main_canvas_region;
        main_canvas_region = nullptr;
    }

    /* SLOT */

    void Engine::on_step()
    {
        for (std::unique_ptr<region> &r : d_active_regions)
        {
            if(main_canvas_region->shape_aabb().intersects(r->shape_aabb()))
            {
                main_canvas_region->on_enter((long) r->winId());
                //r->on_enter((long) r->winId());
                main_canvas_region->on_continuous((long) r->winId());
                //r->on_continuous((long) r->winId());
            }
            else
            {
                main_canvas_region->on_leave((long) r->winId());
                //r->on_leave((long) r->winId());
            }
        }

        main_canvas_region->push_active_regions(d_active_regions);
        main_canvas_region->update();
    }

    /* OVERRIDE */

    void Engine::keyPressEvent(QKeyEvent *event)
    {
        if(event->key() == Qt::Key_Escape)
            stop();
    }

    void Engine::mousePressEvent(QMouseEvent *event)
    {
        is_mouse_pressed = true;
    }

    void Engine::mouseMoveEvent(QMouseEvent *event)
    {
        d_active_regions[0]->update_shape_coords(event->x(), event->y());
    }

    void Engine::mouseReleaseEvent(QMouseEvent *event)
    {
        is_mouse_pressed = false;
    }
    /* End of Engine */
}