

#include "MainWindow.hpp"
#include <SI/SI.hpp>
#include <QPaintEvent>
#include <QPainter>

MainWindow::MainWindow(int width, int height):
    QMainWindow(),
    up_update_worker(std::make_unique<UpdateWorker>()),
    up_qp(std::make_unique<QPainter>()),
    d_width(width),
    d_height(height)
{SIOBJECT(SIREN)
    INFO("Starting Update Loop...");

    connect(up_update_worker.get(), &UpdateWorker::updated, this, &MainWindow::loop);

    connect(up_update_worker.get(), &UpdateWorker::finished, up_update_worker.get(), &UpdateWorker::deleteLater);

    up_update_worker->start();
    INFO("Update Loop started...");
    setGeometry(0, 0, width, height);
}

MainWindow::~MainWindow()
{
    INFO("Closing MainWindow...");
    INFO("MainWindow closed!");
}

void MainWindow::loop(double delta, int fps)
{
    Context::SIContext()->update();

//    DEBUG("Updated with " + std::to_string(fps) + " fps (real delta time: " + std::to_string(delta) + ")");

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
            d_region_representations.insert({region->uuid(), std::make_unique<RegionRepresentation>(region->contour(), region->transform(), 255, 0, 0, 255, region->texture_path())});
        else
        {
            if (region->is_transformed())
                d_region_representations[region->uuid()]->update(region->transform());
        }
    }

    if(!d_region_representations.empty())
    {
        std::vector<glm::vec2> partial_contour;
        // partial stuff
    }

    update();
}

void MainWindow::set_is_running(bool running)
{

}

void MainWindow::draw_background(QPaintEvent* event)
{
    up_qp->setBrush(QColor(0, 0, 0));
    up_qp->drawRect(event->rect());
}

void MainWindow::draw_region_representations(QPaintEvent* event)
{
    for(const auto& [key, val]: d_region_representations)
    {
        up_qp->setBrush(val->color);

        up_qp->drawPolyline(val->poly);
        up_qp->fillPath(val->fill, val->color);
    }
}

void MainWindow::paintEvent(QPaintEvent* event)
{
    up_qp->begin(this);

    draw_background(event);
    draw_region_representations(event);

    up_qp->end();
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    switch (event->key())
    {
        case Qt::Key::Key_Escape:
            close();
        break;

        default:
            return;
    }
}

