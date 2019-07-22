//
// Created by juergen on 22/01/19.
//

#include <QtGui/QPainter>
#include <QKeyEvent>
#include <QPaintEvent>
#include "../include/engine.h"
#include "../include/debug.h"
#include "../include/canvas_region.h"
#include "../include/mouse_region.h"
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

    void Engine::start(bool test)
    {
        d_is_running = true;

        // evaluate whether a main canvas is part of the d_active_regions
        // if so move it from the list to the main_canvas_region variable and delete it from the list
        // if not load default main canvas via its in engine class
        // evaluate main canvas settings

        setMouseTracking(true);
        setWindowState(Qt::WindowFullScreen);

        connect(p_step.get(), &step::on_step, this, &Engine::on_step);
        p_step->start();

        main_canvas_region->setObjectName("main_canvas");

        setCentralWidget(main_canvas_region.get());
        
        show();
    }

    void Engine::stop()
    {
        d_is_running = false;

        if(p_step->isRunning())
        {
            p_step->stop();

            while (!p_step->isFinished());
        }

        if(!d_active_regions.empty())
        {
            for(auto &r : d_active_regions)
            {
                r->close();
                r->on_destroy(1);
                r.release();
            }

            d_active_regions.clear();
        }

        if(!d_region_blueprints.empty())
        {
            for(auto &r : d_region_blueprints)
            {
                r->close();
                r->on_destroy(1);
                r.release();
            }

            d_region_blueprints.clear();
        }

        close();
    }

    void Engine::add_region_template(region *r)
    {
        if(r)
            d_active_regions.push_back(std::unique_ptr<region>(r));
        else
            throw std::runtime_error("Passed argument r is nullptr");
    }

    bool Engine::is_running()
    {
        return d_is_running;
    }

    void Engine::clear_active_regions()
    {
        if(!d_active_regions.empty())
            d_active_regions.clear();
    }

    bool Engine::has_active_regions()
    {
        return !d_active_regions.empty();
    }

    const std::unique_ptr<step> &Engine::i_step() const
    {
        return p_step;
    }

    const std::unique_ptr<canvas> &Engine::i_main_canvas_region() const
    {
        return main_canvas_region;
    }

    /* PRIVATE */

    Engine::Engine() : QMainWindow(), p_step(new step(33.0)), main_canvas_region(new canvas(QPolygon(QVector<QPoint>({QPoint(0, 0), QPoint(0, 1080), QPoint(1920, 1080), QPoint(1920, 0)})), this))
    {}

    Engine::Engine(const Engine &) : QMainWindow(), p_step(new step(33.0)), main_canvas_region(new canvas(QPolygon(QVector<QPoint>({QPoint(0, 0), QPoint(0, 1080), QPoint(1920, 1080), QPoint(1920, 0)})), this))
    {}

    Engine::~Engine() = default;

    /* SLOT */

    void Engine::on_step()
    {
        for (std::unique_ptr<region> &r : d_active_regions)
        {
            if(main_canvas_region->shape_aabb().intersects(r->shape_aabb()))
            {
                //main_canvas_region->on_enter((long) r->winId());
                r->on_enter((long) r->winId());
                //main_canvas_region->on_continuous((long) r->winId());
                r->on_continuous((long) r->winId());
            }
            else
            {
                main_canvas_region->on_leave((long) r->winId());
                r->on_leave((long) r->winId());
            }
        }

        main_canvas_region->push_active_regions(&d_active_regions);
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
        // mouse move
        d_active_regions[0]->update_shape_coords(event->x(), event->y());
    }

    void Engine::mouseReleaseEvent(QMouseEvent *event)
    {
        is_mouse_pressed = false;
    }
    /* End of Engine */
}