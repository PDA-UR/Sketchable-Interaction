//
// Created by juergen on 28/01/19.
//

#include "canvas.h"
#include "../si_debug/debug.h"
#include <QPaintEvent>
#include <QtGui/QPainter>

namespace si
{
    canvas::canvas(const QRegion &s, QWidget *parent) : region(s, parent), q_painter(new QPainter())
    {
        setMouseTracking(true);

        resize(1920, 1080);
        setVisible(true);
        show();
    }

    canvas::canvas(QWidget *parent) : region(QRegion(), parent)
    {}

    void si::canvas::on_enter()
    {
        if (!d_is_standard_lib)
        {
            on_region_enter(1);
        }
        else
        {
            debug::print("Hello From Std Lib Enter!");
        }
    }

    void si::canvas::on_continuous()
    {
        if (!d_is_standard_lib)
        {
            on_region_continuous(1);
        }
        else
        {
            debug::print("Hello From Std Lib Continuous!");
        }
    }

    void si::canvas::on_leave()
    {
        if (!d_is_standard_lib)
        {
            on_region_leave(1);
        }
        else
        {
            debug::print("Hello From Std Lib Leave!");
        }
    }

    void canvas::paintEvent(QPaintEvent *event)
    {
        q_painter->begin(this);

        q_painter->setBrush(QColor(0, 0, 0));
        q_painter->drawRect(event->rect());
        q_painter->setBrush(QColor(0, 255, 0));

        q_painter->end();
    }

    void canvas::mousePressEvent(QMouseEvent *event)
    {
        event->ignore();
    }

    void canvas::mouseReleaseEvent(QMouseEvent *event)
    {
        event->ignore();
    }

    void canvas::mouseMoveEvent(QMouseEvent *event)
    {
        event->ignore();
    }
}

