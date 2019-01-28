//
// Created by juergen on 28/01/19.
//

#include "neutral.h"
#include "../si_debug/debug.h"
#include <QPaintEvent>
#include <QPainter>

namespace si
{
    neutral::neutral(const QRegion &s, QWidget *parent) : region(s, parent), q_painter(new QPainter(this))
    {}

    neutral::neutral(QWidget *parent) : region(parent), q_painter(new QPainter(this))
    {
        setGeometry(50, 50, 50, 50);
        setVisible(true);
        show();
    }

    void neutral::on_enter()
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

    void neutral::on_continuous()
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

    void neutral::on_leave()
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

    void neutral::paintEvent(QPaintEvent *event)
    {
        q_painter->begin(this);

        q_painter->setBrush(QColor(255, 0, 255));
        q_painter->drawRect(event->rect());
        q_painter->setBrush(QColor(0, 255, 0));

        q_painter->end();
    }
}