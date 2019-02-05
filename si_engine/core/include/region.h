//
// Created by juergen on 25/01/19.
//

#ifndef CORE_REGION_H
#define CORE_REGION_H

#include <QWidget>
#include <QPointF>
#include <vector>
#include <functional>

namespace si
{
    extern "C"
    {
    typedef int (*region_callback)(long);
    }

    class region : public QWidget
    { Q_OBJECT
    public:
        region(const QPolygon &s, QWidget *parent= nullptr);
        explicit region(QWidget *parent= nullptr);
        explicit region(region_callback rce, region_callback rcc, region_callback rcl);

        //copy ctor
        region(const region &copy, QWidget *parent= nullptr);

        virtual ~region()
        {
            setParent(nullptr);
        }

        //on interaction callbacks aka the effect (own class appears weird)

        std::function<int(long)> on_region_enter;
        std::function<int(long)> on_region_continuous;
        std::function<int(long)> on_region_leave;

        region_callback on_region_enter_callback;
        region_callback on_region_continuous_callback;
        region_callback on_region_leave_callback;

        // getter
        const QPolygon &shape() const;
        const QRect &shape_aabb() const;

        //setter
        void set_shape(const QPolygon &s);
        bool has_shape();

        // Effect -> the effect which can be applied to others
        // properties
        // capabilities
            // send
            // receive

        void set_enter_callback(const std::function<int(long)> &callback);
        void set_enter_callback(region_callback rc);

        void set_continuous_callback(const std::function<int(long)> &callback);
        void set_continuous_callback(region_callback rc);

        void set_leave_callback(const std::function<int(long)> &callback);
        void set_leave_callback(region_callback rc);

        virtual void update_shape_coords(int x, int y);
        int on_enter(long uuid);
        int on_continuous(long uuid);
        int on_leave(long uuid);

    protected:
        QPolygon d_shape;
        QRect d_shape_aabb;

        bool d_is_visible = false;
        bool d_is_scalable = false;
        bool d_is_standard_lib = false;
        std::vector<long> d_collider_uuids;
    };

    extern "C"
    {

    }
}

#endif //CORE_REGION_H
