

#include "MainWindow.hpp"
#include <SI/SI.hpp>
#include <QPaintEvent>
#include <QDebug>
#include <execution>

MainWindow::MainWindow(int width, int height):
    QMainWindow(),
    up_update_worker(UpdateWorker(60)),
    d_width(width),
    d_height(height)
{SIREN
    INFO("Starting Update Loop...");

    connect(&up_update_worker, &UpdateWorker::updated, this, &MainWindow::loop);
    connect(&up_update_worker, &UpdateWorker::finished, &up_update_worker, &UpdateWorker::deleteLater);

    up_update_worker.start();
    INFO("Update Loop started...");

    setWindowFlags(Qt::WindowStaysOnTopHint);
}

void MainWindow::loop(double delta, int fps)
{
    const auto& regions = Context::SIContext()->region_manager()->regions();

    bool is_present = false;

    for(const auto& [key, val]: d_region_representations)
    {
        if(std::find_if(std::execution::par_unseq, regions.begin(), regions.end(), [&key](const auto& region)
        {
            return key == region->uuid();
        }) == regions.end())
            d_region_representations.erase(key);
    }

    for(const auto& region: regions)
    {
        if(d_region_representations.find(region->uuid()) == d_region_representations.end())
            d_region_representations[region->uuid()] = std::make_unique<RegionRepresentation>(this, region);
        else
            d_region_representations[region->uuid()]->update(region);
    }

    Context::SIContext()->update();
}

void MainWindow::set_is_running(bool running)
{

}