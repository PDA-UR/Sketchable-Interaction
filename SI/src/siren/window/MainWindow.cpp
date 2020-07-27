

#include "MainWindow.hpp"
#include <QPaintEvent>
#include <QDebug>
#include <algorithm>
#include <siren/timing/Timing.hpp>

MainWindow::MainWindow(uint32_t width, uint32_t height, uint32_t target_fps):
    QMainWindow(),
    d_width(width),
    d_height(height),
    d_is_running(true),
    d_target_fps(target_fps)
{
    INFO("Starting Update Loop...");

    INFO("Update Loop started...");

    setWindowTitle("SI");

    Context::SIContext()->set_main_window();

    engine = new QQmlEngine(this);
    engine->setObjectOwnership(engine, QQmlEngine::CppOwnership);
}

void MainWindow::loop()
{
    double last_time = Time::get_time();
    double unprocessed_time = 0.0;
    double frame_counter = 0.0;
    int frames = 0;

    while (d_is_running)
    {
        bool render = false;

        double start_time = Time::get_time();
        double passed_time = start_time - last_time;
        last_time = start_time;

        unprocessed_time += passed_time;
        frame_counter += passed_time;

        if(frame_counter > 1.0)
        {
            INFO(frames);
            frame_counter = 0.0;
            frames = 0;
        }

        while(unprocessed_time > 1.0 / d_target_fps)
        {
            render = true;
            Time::set_time_delta(1.0 / d_target_fps);
            unprocessed_time -= 1.0 / d_target_fps;
        }

        if(render)
        {
            __loop();
            Context::SIContext()->update();

            frames++;
        }
    }

    close();
    deleteLater();
}

void MainWindow::__loop()
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
}

void MainWindow::pause()
{
    WARN("PAUSING OF RENDERING IS UNIMPLEMENTED");
}
