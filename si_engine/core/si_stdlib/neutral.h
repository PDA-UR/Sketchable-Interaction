//
// Created by juergen on 28/01/19.
//

#ifndef CORE_NEUTRAL_H
#define CORE_NEUTRAL_H

#include "../si_interaction/region.h"

namespace si
{
    class neutral : public region
    {Q_OBJECT
    protected:
        void paintEvent(QPaintEvent *event) override;

    public:
        neutral(const QRegion &s, QWidget *parent = 0);
        neutral(QWidget * parent = nullptr);

        void on_enter() override;
        void on_continuous() override;
        void on_leave() override;

    private:
        QPainter *q_painter;
    };
}


#endif //CORE_NEUTRAL_H
