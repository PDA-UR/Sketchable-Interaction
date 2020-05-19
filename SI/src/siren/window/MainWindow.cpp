

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

    d_reg_reps.erase(std::remove_if(d_reg_reps.begin(), d_reg_reps.end(), [&](auto& rep)
    {
        return std::find_if(std::execution::par_unseq, regions.begin(), regions.end(), [&](auto& reg)
        {
            return reg->uuid() == rep->uuid();
        }) == regions.end();
    }), d_reg_reps.end());

    std::for_each(std::execution::seq, regions.begin(), regions.end(), [&](std::shared_ptr<Region>& reg)
    {
        auto it = std::find_if(std::execution::par_unseq, d_reg_reps.begin(), d_reg_reps.end(), [&](std::unique_ptr<RegionRepresentation>& rep)
        {
            return reg->uuid() == rep->uuid();
        });

        if(it == d_reg_reps.end())
        {
            d_reg_reps.push_back(std::make_unique<RegionRepresentation>(this, reg));

            switch (d_reg_reps.back()->type())
            {
                // automatically created first and therefore last in the widget stack
                case SI_TYPE_CANVAS:
                    break;

                // automatically visualized ontop due to use of OS cursor
                case SI_TYPE_CURSOR:
                case SI_TYPE_MOUSE_CURSOR:
                    break;

                // except for cursors, these effect type are intended to be the highest placed widgets in the widget stack
                case SI_TYPE_ENTRY:
                case SI_TYPE_DIRECTORY:
                case SI_TYPE_TEXT_FILE:
                case SI_TYPE_IMAGE_FILE:
                case SI_TYPE_UNKNOWN_FILE:
                    break;

                // case for regularly drawn regions
                default:
                {
                    // look for a file
                    auto it = std::find_if(std::execution::par_unseq, d_reg_reps.begin(), d_reg_reps.end(), [&](auto& rep)
                    {
                        return rep->type() == SI_TYPE_ENTRY
                               || rep->type() == SI_TYPE_DIRECTORY
                               || rep->type() == SI_TYPE_TEXT_FILE
                               || rep->type() == SI_TYPE_IMAGE_FILE
                               || rep->type() == SI_TYPE_UNKNOWN_FILE;
                    });

                    if(it != d_reg_reps.end())
                        d_reg_reps.back()->stackUnder(it->get());

                    break;
                }
            }
        }
        else
            it->get()->update(reg);
    });

    Context::SIContext()->update();
}
