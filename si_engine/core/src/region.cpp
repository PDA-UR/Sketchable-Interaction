//
// Created by juergen on 25/01/19.
//

#include "../include/region.h"
#include "../include/debug.h"
#include "../include/util.h"

namespace si
{
    region::region(const QPolygon &s, QWidget *parent) : QWidget(parent), d_shape(s)
    {
        d_shape_aabb = d_shape.boundingRect();
    }

    region::region(QWidget * parent) : QWidget(parent)
    {}

    region::region(region_callback rce, region_callback rcc, region_callback rcl)
    {
        set_enter_callback(rce);
        set_continuous_callback(rcc);
        set_leave_callback(rcl);
    }

    region::region(const region &copy, QWidget *parent) : QWidget(parent)
    {}

    const QPolygon &region::shape() const
    {
        return d_shape;
    }

    const QRect &region::shape_aabb() const
    {
        return d_shape_aabb;
    }

    void region::set_shape(const QPolygon &s)
    {
        d_shape = s;
        d_shape_aabb = d_shape.boundingRect();
    }

    bool region::has_shape()
    {
        return !d_shape.isEmpty();
    }

    void region::set_enter_callback(const std::function<int(long)> &callback)
    {
        on_region_enter = callback;
    }

    void region::set_continuous_callback(const std::function<int(long)> &callback)
    {
        on_region_continuous = callback;
    }

    void region::set_leave_callback(const std::function<int(long)> &callback)
    {
        on_region_leave = callback;
    }

    void region::set_enter_callback(region_callback rc)
    {
        on_region_enter = rc;
    }

    void region::set_continuous_callback(region_callback rc)
    {
        on_region_continuous = rc;
    }

    void region::set_leave_callback(region_callback rc)
    {
        on_region_leave = rc;
    }

    void region::update_shape_coords(int x, int y) {}

    int region::on_enter(long uuid)
    {
        if(on_region_enter)
        {
            if(contains(d_collider_uuids, uuid) == -1)
            {
                d_collider_uuids.push_back(uuid);

                on_region_enter(uuid);
            }
        }
    }

    int region::on_continuous(long uuid)
    {
        if(contains(d_collider_uuids, uuid) > -1)
        {
            on_region_continuous(uuid);
        }
    }

    int region::on_leave(long uuid)
    {
        int index = contains(d_collider_uuids, uuid);

        if(index > -1)
        {
            d_collider_uuids.erase(d_collider_uuids.begin() + index);

            on_region_leave(uuid);
        }
    }

}


