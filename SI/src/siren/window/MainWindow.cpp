

#include "MainWindow.hpp"
#include <SI/SI.hpp>
#include <QPaintEvent>
#include <QDebug>

MainWindow::MainWindow(int width, int height):
    QMainWindow(),
    up_update_worker(UpdateWorker()),
    up_qp(QPainter()),
    d_width(width),
    d_height(height)
{SIREN
    INFO("Starting Update Loop...");

    connect(&up_update_worker, &UpdateWorker::updated, this, &MainWindow::loop);
    connect(&up_update_worker, &UpdateWorker::finished, &up_update_worker, &UpdateWorker::deleteLater);

    up_update_worker.start();
    INFO("Update Loop started...");
}

void MainWindow::loop(double delta, int fps)
{
    const auto& regions = Context::SIContext()->region_manager()->regions();

    bool is_present = false;

    for(const auto& [key, val]: d_region_representations)
    {
        is_present = false;

        for(const auto& region: regions)
        {
            if(key == region->uuid())
            {
                is_present = true;
                break;
            }
        }

        if(!is_present)
            d_region_representations.erase(key);
    }

    for(const auto& region: regions)
    {
        if(d_region_representations.find(region->uuid()) == d_region_representations.end())
            d_region_representations.insert({region->uuid(), std::make_unique<RegionRepresentation>(this, region)});
        else
            d_region_representations[region->uuid()]->update(region);
    }

    update();
    Context::SIContext()->update();
}

void MainWindow::set_is_running(bool running)
{

}

void MainWindow::draw_background(QPaintEvent* event)
{
    up_qp.setBrush(QColor(0, 0, 0));
    up_qp.drawRect(event->rect());
}

void MainWindow::draw_region_representations(QPaintEvent* event)
{
    for(const auto& [key, val]: d_region_representations)
    {
        if(val->name() == "MouseCursor" || val->name() == "stdCanvas")
            continue;

        up_qp.setBrush(val->color());
        up_qp.fillPath(val->fill(), val->color());
    }
}

void MainWindow::paintEvent(QPaintEvent* event)
{
    up_qp.begin(this);

    draw_background(event);
    draw_region_representations(event);

    const auto& partial_regions = Context::SIContext()->region_manager()->partial_regions();

    for(auto& [key, partial_region]: partial_regions)
    {
        QPolygonF partial_poly;

        up_qp.setBrush(QColor(255, 255, 255));

        for(auto& p: partial_region)
            partial_poly << QPointF(p.x, p.y);

        up_qp.drawPolyline(partial_poly);
    }

    up_qp.end();
}
