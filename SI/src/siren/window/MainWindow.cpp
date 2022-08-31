

#include "MainWindow.hpp"
#include <QDebug>
#include <algorithm>
#include <Qt>
#include <siren/timing/Timing.hpp>
#include <debug/Print.hpp>

MainWindow::MainWindow(uint32_t width, uint32_t height, uint32_t target_fps):
    QGraphicsView(),
    d_width(width),
    d_height(height),
    d_is_running(true),
    d_target_fps(60),
    d_drawing_pen_color(Context::SIContext()->pen_color()),
    p_scene(new QGraphicsScene())
{
    setWindowTitle("SI");

    Context::SIContext()->set_main_window();

    this->setInteractive(false);

    d_engine = new QQmlEngine();
    d_engine->setObjectOwnership(d_engine, QQmlEngine::CppOwnership);

    d_qmlcontext = new QQmlContext(d_engine);

    p_scene->setSceneRect(0, 0, Context::SIContext()->width(), Context::SIContext()->height());

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

//    setViewport(new QOpenGLWidget);
    setScene(p_scene);
//    setRenderHint(QPainter::Antialiasing);
}

void MainWindow::loop()
{
    INFO("Update Loop started...");

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
            Context::SIContext()->push_fps(frames, d_target_fps);
            frame_counter = 0.0;
            frames = 0;
        }

        while(unprocessed_time > 1.0 / d_target_fps)
        {
            render = true;
            Time::set_time_delta(1.0 / d_target_fps);
            unprocessed_time -= 1.0 / d_target_fps;
        }

//        if(render)
//        {
            Context::SIContext()->update();
            __loop();
            frames++;
//        }
    }

    close();
    p_scene->deleteLater();
}

void MainWindow::__loop()
{
    handle_region_representations();
    handle_partial_region_representations();
}

void MainWindow::pause()
{
    WARN("PAUSING OF RENDERING IS UNIMPLEMENTED");
}

QQmlEngine* MainWindow::engine()
{
    return d_engine;
}

void MainWindow::handle_region_representations()
{
    auto& regions = Context::SIContext()->region_manager()->regions();

    for(auto& reg: regions)
    {
        auto it = std::find_if(d_reg_reps.begin(), d_reg_reps.end(), [&](RegionRepresentation* rep)
        {
            return reg->uuid() == rep->uuid();
        });

        if(it == d_reg_reps.end())
        {
            d_reg_reps.push_back(new RegionRepresentation(d_qmlcontext, reg, this));
            p_scene->addItem(d_reg_reps.back());
        }
        else
        {
            (*it)->update(reg);
        }
    }

    d_reg_reps.erase(std::remove_if(d_reg_reps.begin(), d_reg_reps.end(), [&](RegionRepresentation* rep)
    {
        auto it = std::find_if(regions.begin(), regions.end(), [&](auto& reg)
        {
            return reg->uuid() == rep->uuid();
        });

        if (it != regions.end())
            return false;

        p_scene->removeItem(rep);

        delete rep;
        rep = nullptr;
        return true;

    }), d_reg_reps.end());


}

void MainWindow::handle_partial_region_representations()
{
    d_par_reg_reps.erase(std::remove_if(d_par_reg_reps.begin(), d_par_reg_reps.end(), [&](PartialRegionRepresentation* prep)
    {
        for(auto& [k, v]: Context::SIContext()->region_manager()->partial_regions())
            if(k == prep->id())
                return false;

        p_scene->removeItem(prep);
        delete prep;
        prep = nullptr;

        return true;
    }), d_par_reg_reps.end());

    for(auto& [k, v]: Context::SIContext()->region_manager()->partial_regions())
    {
        const auto& source = k;
        const auto& path = v;

        auto it = std::find_if(d_par_reg_reps.begin(), d_par_reg_reps.end(), [&](PartialRegionRepresentation* prep)
        {
            return source == prep->id();
        });

        if(it == d_par_reg_reps.end())
        {
            PartialRegionRepresentation* part_reg;
            if(d_cursor_stroke_data.find(source) != d_cursor_stroke_data.end())
            {
                part_reg = new PartialRegionRepresentation(source, path, std::get<0>(d_cursor_stroke_data[source]), std::get<1>(d_cursor_stroke_data[source]));
                d_cursor_stroke_data.erase(source);
            }
            else
                part_reg = new PartialRegionRepresentation(source, path);

            d_par_reg_reps.push_back(part_reg);
            p_scene->addItem(d_par_reg_reps.back());
        }
        else
        {
            (*it)->update(path);
        }
    }
}

void MainWindow::set_cursor_stroke_width_by_cursor_id(const std::string &cursor_id, int stroke_width)
{
    if(d_cursor_stroke_data.find(cursor_id) != d_cursor_stroke_data.end())
        d_cursor_stroke_data[cursor_id] = std::tuple<int, glm::vec4>(stroke_width, std::get<1>(d_cursor_stroke_data[cursor_id]));
    else
        d_cursor_stroke_data[cursor_id] = std::tuple<int, glm::vec4>(stroke_width, glm::vec4(72, 79, 81, 255));
}

void MainWindow::set_cursor_stroke_color_by_cursor_id(const std::string &cursor_id, const glm::vec4 &color)
{
    if(d_cursor_stroke_data.find(cursor_id) != d_cursor_stroke_data.end())
        d_cursor_stroke_data[cursor_id] = std::tuple<int, glm::vec4>(std::get<0>(d_cursor_stroke_data[cursor_id]), color);
    else
        d_cursor_stroke_data[cursor_id] = std::tuple<int, glm::vec4>(4, color);
}
