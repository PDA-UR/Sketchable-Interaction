//
// Created by juergen on 28/01/19.
//

#ifndef CORE_NEUTRAL_H
#define CORE_NEUTRAL_H

#include "region.h"

namespace si
{
    class mouse_region : public region
    {Q_OBJECT

    protected:
        void paintEvent(QPaintEvent *event) override;

    public:
        mouse_region(const QPolygon &s, QWidget *parent = nullptr);
        mouse_region(QWidget *parent = nullptr);

        //copy ctor
        mouse_region(const mouse_region &copy, QWidget *parent= nullptr);

        ~mouse_region() = default;

        int on_enter_for_callback(long uuid);
        int on_continuous_for_callback(long uuid);
        int on_leave_for_callback(long uuid);
        int on_create_for_callback(long uuid);
        int on_destroy_for_callback(long uuid);

        //void update_shape_coords(int x, int y) override;

    private:
        const int C_DEFAULT_RADIUS = 10;
        const int C_DEFAULT_NUM_SEGMENTS = 30;

        const QPolygon circle(int x, int y, int radius, int num_segments);
    };
}


#endif //CORE_NEUTRAL_H
