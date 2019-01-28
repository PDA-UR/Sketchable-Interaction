//
// Created by juergen on 28/01/19.
//

#ifndef CORE_CANVAS_H
#define CORE_CANVAS_H

#include <QWidget>

#include "../si_interaction/region.h"

namespace si
{
    class canvas : public region
    { Q_OBJECT
    protected:
        void paintEvent(QPaintEvent *event) override;

        void mousePressEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;

    public:
        canvas(const QRegion &s, QWidget *parent = 0);
        canvas(QWidget * parent = 0);

        void on_enter() override;
        void on_continuous() override;
        void on_leave() override;

    private:
        QPainter *q_painter;
    };
}

#endif //CORE_CANVAS_H
