#include <iostream>

#include "engine.h"
#include "timing.h"
#include "../region/region.h"
#include "../scripting/plugins_collector.h"

void engine::load_plugins()
{
    std::vector<std::string> files, classes;

    plugins_collector::collect(files);

    for(auto& path : files)
    {
        std::string base_filename = path.substr(path.find_last_of("/\\") + 1);
        std::string module_name = base_filename.substr(0, base_filename.find_last_of('.'));
        std::string rel_path = "plugins/" + base_filename;

        script.load_class_names(classes, path);

        for(auto& ref : classes)
            add_region_plugin(script.region_plugin(module_name, rel_path, ref));
    }
}

void engine::add_region_plugin(boost::python::object pyObject)
{
    SIRegion r;

    r.set_pyregion(pyObject);
    r.on_enter();
}

void engine::start()
{
    if(is_running)
        return;

    load_plugins();

    run();
}

void engine::run()
{
    is_running = true;

    double last_time = time::get_time();
    double unprocessed_time = 0.0;
    double frame_counter = 0.0;

    int frames = 0;

    while(is_running)
    {
        bool render = false;

        double start_time = time::get_time();
        double passed_time = start_time - last_time;

        last_time += passed_time;
        unprocessed_time += passed_time;
        frame_counter += passed_time;

        if(frame_counter > 1.0)
        {
            std::cout << frames << std::endl;

            frame_counter = 0.0;
            frames = 0;
        }

        while (unprocessed_time > frame_time)
        {
            render = true;
            time::set_time_delta(frame_time);
            unprocessed_time -= frame_time;
        }

        if(render)
        {
            frames++;
        }
    }
}

void engine::stop()
{
    if(!is_running)
        return;

    is_running = false;
}
