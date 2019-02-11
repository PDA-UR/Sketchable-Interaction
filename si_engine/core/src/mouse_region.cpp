//
// Created by juergen on 28/01/19.
//

#include "../include/mouse_region.h"
#include "../include/debug.h"
#include <QPaintEvent>
#include <QPainter>
#include <math.h>

namespace si
{
    neutral::neutral(const QPolygon &s, QWidget *parent) : region(s, parent)
    {

    }

    neutral::neutral(QWidget *parent) : region(parent)
    {
        setObjectName("Mouse Region");
        setVisible(false);
        d_is_standard_lib = true;
    }

    neutral::neutral(const neutral &copy, QWidget *parent) : region(parent)
    {}

    int neutral::on_enter_for_callback(long uuid)
    {
        if (!d_is_standard_lib)
        {
            on_region_enter(1);
        }
        else
        {
            debug::print("Hello From Std Lib Enter!");
        }

        return 0;
    }

    int neutral::on_continuous_for_callback(long uuid)
    {
        if (!d_is_standard_lib)
        {
            on_region_continuous(1);
        }
        else
        {
            debug::print("Hello From Std Lib Continuous!");
        }

        return 0;
    }

    int neutral::on_leave_for_callback(long uuid)
    {
        if (!d_is_standard_lib)
        {
            on_region_leave(1);
        }
        else
        {
            debug::print("Hello From Std Lib Leave!");
        }

        return 0;
    }

    void neutral::update_shape_coords(int x, int y)
    {
        set_shape(circle(x, y, C_DEFAULT_RADIUS, C_DEFAULT_NUM_SEGMENTS));
    }

    // OVERRIDE

    void neutral::paintEvent(QPaintEvent *event)
    {
        QPainter q_painter;
        q_painter.begin(this);



        q_painter.end();
    }

    // PRIVATE

    const QPolygon neutral::circle(int cx, int cy, int radius, int num_segments)
    {
        QPolygon p;

        for (int i = 0; i < num_segments; i++)
        {
            float theta = 2.0f * 3.1415926f * (float) i / (float) num_segments;


            int x = radius * cos(theta);
            int y = radius * sin(theta);

            p << QPoint(x + cx, y + cy);
        }

        return p;
    }
}