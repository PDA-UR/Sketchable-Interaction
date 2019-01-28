//
// Created by juergen on 25/01/19.
//

#include "region.h"

namespace si
{
    region::region(const QRegion &s, QWidget * parent) : QWidget(parent), d_shape(s)
    {}

    region::region(QWidget * parent) : QWidget(parent)
    {}

    const QRegion &region::shape()
    {
        return d_shape;
    }

    void region::set_shape(const QRegion &s)
    {
        d_shape = s;
    }

    bool region::has_shape()
    {
        return !d_shape.isEmpty();
    }

    void region::on_enter()
    {}

    void region::on_continuous()
    {}

    void region::on_leave()
    {}
}


