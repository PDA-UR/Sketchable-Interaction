//
// Created by juergen on 22/01/19.
//

#include <QtGui/QPainter>
#include <QKeyEvent>
#include <QPaintEvent>
#include "engine.h"
#include "../si_debug/debug.h"
#include "signal_handler.h"
#include "../si_stdlib/canvas.h"
#include "../si_stdlib/neutral.h"
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

        setCentralWidget(main_canvas_region);
        mouse_region = new neutral(main_canvas_region);

        show();
    }

    void Engine::stop()
    {
        close();
    }

    /* PRIVATE */

    Engine::Engine() : QMainWindow(), p_step(new step(33.0)), main_canvas_region(new canvas(QRegion(QPolygon(QVector<QPoint>({QPoint(0, 0), QPoint(0, 1080), QPoint(1920, 1080), QPoint(1920, 0)}))), this))
    {
        signal(SIGINT, signal_handler::handle);
    }

    Engine::Engine(const Engine &) : QMainWindow(), p_step(new step(33.0)), main_canvas_region(new canvas(QRegion(QPolygon(QVector<QPoint>({QPoint(0, 0), QPoint(0, 1080), QPoint(1920, 1080), QPoint(1920, 0)}))), this))
    {
        signal(SIGINT, signal_handler::handle);
    }

    Engine::~Engine()
    {
        p_step->stop();

        while (!p_step->isFinished());

        delete p_step;
        p_step = nullptr;

        main_canvas_region->close();
        delete main_canvas_region;
        main_canvas_region = nullptr;
    }

    /* SLOT */
    void Engine::on_step()
    {
        //main_canvas_region->update();
    }

    /* OVERRIDE */

    void Engine::keyPressEvent(QKeyEvent *event)
    {
        if(event->key() == Qt::Key_Escape)
            stop();
    }

    void Engine::mousePressEvent(QMouseEvent *event)
    {
        // construct new region for mouse cursor and LINK the regions
        // Link:
            // mouse pos -> color at pos in canvas
            // check if canvas is capable of that
            // check if mouse region is capable of that

            //grab().createMaskFromColor(QColor(255, 255, 255), Qt::MaskMode::MaskInColor).save("test.png");
    }

    void Engine::mouseMoveEvent(QMouseEvent *event)
    {
        debug::print(event->pos().x(), event->pos().y());

        mouse_region->setGeometry(event->x() - mouse_region->width() / 2, event->y() - mouse_region->height() / 2, mouse_region->width(), mouse_region->height());
    }

    void Engine::mouseReleaseEvent(QMouseEvent *event)
    {

        // destruct new region for mouse cursor
    }
    /* End of Engine */
}