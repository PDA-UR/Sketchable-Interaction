//
// Created by juergen on 25/01/19.
//

#include <region.h>

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

    region::region(region_callback rce, region_callback rcc, region_callback rcl, region_callback rocc, region_callback rdc)
    {
        set_enter_callback(rce);
        set_continuous_callback(rcc);
        set_leave_callback(rcl);
        set_create_callback(rocc);
        set_destroy_callback(rdc);
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

    void region::update_shape_coords(int x, int y)
    {
        // for each point in shape move by x and y
        // probably needs capability check

        // check if is a main interaction source
        // else check if capable to be moved (via interaction, link, etc.)

        if (d_is_main_interaction_source)
        {
            QPolygon qp;

            for (const QPoint &p : d_shape)
            {
                qp << QPoint(p.x() + x - last_x, p.y() + y - last_y);
            }

            set_shape(qp);
        }
        // else check capability

        last_x = x;
        last_y = y;
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

    void region::set_create_callback(const std::function<int(long)> &callback)
    {
        on_region_create = callback;
    }

    void region::set_destroy_callback(const std::function<int(long)> &callback)
    {
        on_region_destroy = callback;
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

    void region::set_create_callback(region_callback rc)
    {
        on_region_create = rc;
    }

    void region::set_destroy_callback(region_callback rc)
    {
        on_region_destroy = rc;
    }

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

        return 0;
    }

    int region::on_continuous(long uuid)
    {
        if (on_region_continuous)
        {
            if(contains(d_collider_uuids, uuid) > -1)
            {
                on_region_continuous(uuid);
            }
        }

        return 0;
    }

    int region::on_leave(long uuid)
    {
        if (on_region_leave)
        {
            int index = contains(d_collider_uuids, uuid);

            if(index > -1)
            {
                d_collider_uuids.erase(d_collider_uuids.begin() + index);

                on_region_leave(uuid);
            }
        }

        return 0;
    }

    int region::on_create(long uuid)
    {
        if(on_region_create)
        {
            on_region_create(uuid);
        }

        return 0;
    }

    int region::on_destroy(long uuid)
    {
        if(on_region_destroy)
        {
            on_region_destroy(uuid);
        }

        return 0;
    }

    const QColor &region::region_color() const
    {
        return d_region_color;
    }

    void region::set_region_color(int r, int g, int b, int a)
    {
        d_region_color = QColor(r, g, b, a);
    }

    /*
    const region &region::icon_region() const
    {
        return ;
    }
    */

    void region::set_icon_region(const std::string &icon_path)
    {

    }

    const bool region::is_blueprint() const
    {
        return d_is_blueprint;
    }

    void region::set_blueprint(bool flag)
    {
        d_is_blueprint = flag;
    }

    void region::set_as_main_canvas()
    {
        d_is_main_canvas = true;
    }

    void region::load_default_canvas()
    {
        // TODO
    }

    void region::set_fullscreen(bool fullscreen)
    {
        // TODO
    }

    void region::set_as_main_interaction_source()
    {
        d_is_main_interaction_source = true;
    }

    void region::load_default_interaction_source()
    {
        // TODO
    }
}


