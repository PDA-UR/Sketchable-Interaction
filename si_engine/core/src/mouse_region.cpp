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
    mouse_region::mouse_region(const QPolygon &s, QWidget *parent) : region(s, parent)
    {

    }

    mouse_region::mouse_region(QWidget *parent) : region(parent)
    {
        setObjectName("Mouse Region");
        setVisible(false);
        d_is_standard_lib = true;
    }

    mouse_region::mouse_region(const mouse_region &copy, QWidget *parent) : region(parent)
    {}

    int mouse_region::on_enter_for_callback(long uuid)
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

    int mouse_region::on_continuous_for_callback(long uuid)
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

    int mouse_region::on_leave_for_callback(long uuid)
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

    int mouse_region::on_create_for_callback(long uuid)
    {

        debug::print("Hello From Std Lib Create!");

        return 0;
    }

    int mouse_region::on_destroy_for_callback(long uuid)
    {

        debug::print("Hello From Std Lib Destroy!");

        return 0;
    }

    // OVERRIDE

    void mouse_region::paintEvent(QPaintEvent *event)
    {
        QPainter q_painter;

        q_painter.begin(this);

        q_painter.end();
    }

    // PRIVATE

    const QPolygon mouse_region::circle(int cx, int cy, int radius, int num_segments)
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