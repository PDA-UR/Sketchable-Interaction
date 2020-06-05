

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

    setWindowTitle("SI");

    engine = new QQmlEngine(this);
    engine->setObjectOwnership(engine, QQmlEngine::CppOwnership);
}

void MainWindow::loop(double delta, uint32_t fps)
{
    auto& regions = Context::SIContext()->region_manager()->regions();

    d_reg_reps.erase(std::remove_if(d_reg_reps.begin(), d_reg_reps.end(), [&](RegionRepresentation* rep)
    {
        auto it = std::find_if(regions.begin(), regions.end(), [&](auto& reg)
        {
            return reg->uuid() == rep->uuid();
        });

        if(it == regions.end())
        {
            delete rep;
            rep = nullptr;
            return true;
        }

        return false;
    }), d_reg_reps.end());

    for(auto& reg: regions)
    {
        auto it = std::find_if(d_reg_reps.begin(), d_reg_reps.end(), [&](RegionRepresentation* rep)
        {
            return reg->uuid() == rep->uuid();
        });

        if(it == d_reg_reps.end())
        {
            d_reg_reps.push_back(new RegionRepresentation(this, engine, reg));

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
                    auto it2 = std::find_if(d_reg_reps.begin(), d_reg_reps.end(), [&](RegionRepresentation* rep)
                    {
                        return rep->type() == SI_TYPE_ENTRY
                               || rep->type() == SI_TYPE_DIRECTORY
                               || rep->type() == SI_TYPE_TEXT_FILE
                               || rep->type() == SI_TYPE_IMAGE_FILE
                               || rep->type() == SI_TYPE_UNKNOWN_FILE;
                    });

                    if(it2 != d_reg_reps.end())
                        d_reg_reps.back()->stackUnder(*it2);

                    break;
                }
            }
        }
        else
            (*it)->update(reg);
    }

    update();
    Context::SIContext()->update();
}

void MainWindow::pause()
{
    WARN("PAUSING OF RENDERING IS UNIMPLEMENTED");
}