//
// Created by juergen on 28/01/19.
//

#ifndef CORE_CANVAS_H
#define CORE_CANVAS_H

#include <QWidget>

#include "region.h"
#include <vector>
#include <memory>
#include <algorithm>

namespace si
{
    class canvas : public region
    { Q_OBJECT

    protected:
        void paintEvent(QPaintEvent *event) override;
        void closeEvent(QCloseEvent *event) override;

    public:
        canvas(const QPolygon &s, QWidget *parent= nullptr);
        canvas(QWidget * parent = nullptr);
        canvas(const canvas &copy, QWidget *parent= nullptr);
        virtual ~canvas() = default;

        int on_enter_for_callback(long uuid);
        int on_continuous_for_callback(long uuid);
        int on_leave_for_callback(long uuid);

        void push_active_regions(std::vector<std::unique_ptr<region>> *active_regions);

    private:
        std::vector<std::unique_ptr<region>> *d_active_regions = nullptr;
    };
}

#endif //CORE_CANVAS_H
