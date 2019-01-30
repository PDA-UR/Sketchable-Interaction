//
// Created by juergen on 28/01/19.
//

#include "canvas_region.h"
#include "../si_debug/debug.h"
#include <QPaintEvent>
#include <QtGui/QPainter>
#include "../si_util/util.h"
#include <QApplication>
#include <QDesktopWidget>
#include <stdlib.h>

namespace si
{
    canvas::canvas(const QPolygon &s, QWidget *parent) : region(s, parent), q_painter(new QPainter())
    {
        setMouseTracking(true);
        setCursor(Qt::BlankCursor);

        //resize(2560, 1440);
        resize(1920, 1080);
        // hack for this screen QRect rec = QApplication::desktop()->screenGeometry(); is buggy due to multi monitor setup

        setVisible(true);
        d_is_standard_lib = true;

        //set_shape(QPolygon(QRect(QPoint(0, 0), QPoint(2560, 1440))));
        set_shape(QPolygon(QRect(QPoint(0, 0), QPoint(1920, 1080))));

        set_enter_callback(std::bind(&canvas::on_enter_for_callback, this, std::placeholders::_1));
        set_continuous_callback(std::bind(&canvas::on_continuous_for_callback, this, std::placeholders::_1));
        set_leave_callback(std::bind(&canvas::on_leave_for_callback, this, std::placeholders::_1));

        show();
    }

    canvas::canvas(QWidget *parent) : region(QPolygon(), parent)
    {}

    int si::canvas::on_enter_for_callback(long uuid)
    {
        debug::print("Hello There Enter", uuid);
    }

    int si::canvas::on_continuous_for_callback(long uuid)
    {
        debug::print("Hello There Continuous", uuid);
    }

    int si::canvas::on_leave_for_callback(long uuid)
    {
        debug::print("Hello There Leave", uuid);
    }

    void canvas::push_active_regions(std::vector<std::unique_ptr<region>> &active_regions)
    {
        d_active_regions = &active_regions;
    }

    void canvas::paintEvent(QPaintEvent *event)
    {
        q_painter->begin(this);

        q_painter->setBrush(QColor(0, 0, 0));
        q_painter->drawRect(event->rect());
        q_painter->setBrush(QColor(0, 255, 0));
        q_painter->setPen(QPen(QColor(0, 255, 0), 3));

        if(d_active_regions)
        {
            for(std::unique_ptr<region> &r : *d_active_regions)
            {
                QPainterPath p;

                p.addPolygon(r->shape());

                q_painter->drawPath(p);
            }

            for(QPoint &p : d_shape)
            {
                q_painter->drawEllipse(p, 20, 20);
            }
        }

        q_painter->end();
    }

    void canvas::closeEvent(QCloseEvent *event)
    {
        d_active_regions = nullptr;
        delete d_active_regions;
    }
}

