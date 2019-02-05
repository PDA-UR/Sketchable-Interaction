//
// Created by juergen on 28/01/19.
//

#ifndef CORE_NEUTRAL_H
#define CORE_NEUTRAL_H

#include "region.h"

namespace si
{
    class neutral : public region
    {Q_OBJECT

    protected:
        void paintEvent(QPaintEvent *event) override;

    public:
        neutral(const QPolygon &s, QWidget *parent = nullptr);
        neutral(QWidget *parent = nullptr);

        //copy ctor
        neutral(const neutral &copy, QWidget *parent= nullptr);

        ~neutral() override;

        int on_enter_for_callback(long uuid);
        int on_continuous_for_callback(long uuid);
        int on_leave_for_callback(long uuid);

        void update_shape_coords(int x, int y) override;

    private:
        QPainter *q_painter;

        const int C_DEFAULT_RADIUS = 10;
        const int C_DEFAULT_NUM_SEGMENTS = 30;

        const QPolygon circle(int x, int y, int radius, int num_segments);
    };
}


#endif //CORE_NEUTRAL_H
