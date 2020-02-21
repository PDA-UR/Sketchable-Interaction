

#include "MainWindow.hpp"
#include <SI/SI.hpp>
#include <QPaintEvent>
#include <QDebug>

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

    Context::SIContext()->update();
}

void MainWindow::set_is_running(bool running)
{

}