

#include "MainWindow.hpp"
#include <QPaintEvent>
#include <QDebug>
#include <execution>

MainWindow::MainWindow(uint32_t width, uint32_t height):
    QMainWindow(),
    up_update_worker(UpdateWorker(60)),
    d_width(width),
    d_height(height)
{
    INFO("Starting Update Loop...");

    connect(&up_update_worker, &UpdateWorker::updated, this, &MainWindow::loop);
    connect(&up_update_worker, &UpdateWorker::finished, &up_update_worker, &UpdateWorker::deleteLater);

    up_update_worker.start();
    INFO("Update Loop started...");

    setWindowFlags(Qt::WindowStaysOnBottomHint);
    setWindowTitle("SI");
}

void MainWindow::loop(double delta, uint32_t fps)
{
    auto& regions = Context::SIContext()->region_manager()->regions();

    std::for_each(std::execution::par_unseq, d_region_representations.begin(), d_region_representations.end(), [&](auto& representation)
    {
        if(std::find_if(std::execution::par_unseq, regions.begin(), regions.end(), [&](const auto& region)
        {
            return representation.first == region->uuid();
        }) == regions.end())
            d_region_representations.erase(representation.first);
    });

    std::for_each(std::execution::seq, regions.begin(), regions.end(), [&](auto& region)
    {
        if(d_region_representations.find(region->uuid()) == d_region_representations.end())
            d_region_representations[region->uuid()] = std::make_unique<RegionRepresentation>(this, region);
        else
            d_region_representations[region->uuid()]->update(region);
    });

    Context::SIContext()->update();
}

void MainWindow::set_is_running(bool running)
{

}