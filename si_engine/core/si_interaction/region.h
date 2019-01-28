//
// Created by juergen on 25/01/19.
//

#ifndef CORE_REGION_H
#define CORE_REGION_H

#include <QWidget>
#include <QPointF>
#include <vector>

namespace si
{
    extern "C"
    {
    typedef int (*region_callback)(long);
    }

    class region : public QWidget
    { Q_OBJECT
    public:
        region(const QRegion &s, QWidget *parent= nullptr);
        explicit region(QWidget *parent= nullptr);

        //on interaction callbacks aka the effect (own class appears weird)
        region_callback on_region_enter;
        region_callback on_region_continuous;
        region_callback on_region_leave;

        // getter
        const QRegion &shape();

        //setter
        void set_shape(const QRegion &s);
        bool has_shape();



        // Effect -> the effect which can be applied to others
        // properties
        // capabilities
            // send
            // receive

        virtual void on_enter();
        virtual void on_continuous();
        virtual void on_leave();

    protected:
        QRegion d_shape;

        bool d_is_visible = false;
        bool d_is_scalable = false;
        bool d_is_standard_lib = false;
    };
}

#endif //CORE_REGION_H
