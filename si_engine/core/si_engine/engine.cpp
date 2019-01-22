//
// Created by juergen on 22/01/19.
//

#include "engine.h"
#include "../si_debug/debug.h"
#include "timing.h"

namespace si
{

    /* PUBLIC */
    engine * engine::s_instance = nullptr;
    const double engine::sc_DESIRED_FPS = 60.0;

    si::engine *si::engine::__instance__()
    {
        static CGuard cg;

        if(!s_instance)
            s_instance = new engine();

        return s_instance;
    }

    void si::engine::start()
    {
        if (d_is_running)
            return;

        run();
    }

    void si::engine::pause()
    {

    }

    void si::engine::stop()
    {
        if (!d_is_running)
            return;

        d_is_running = false;
    }

    void si::engine::resume()
    {

    }

    void engine::register_plugin(void *plugin)
    {

    }

    /*PRIVATE*/
    si::engine::engine() :
        d_frame_time(1.0 / 60.0),
        d_is_running(false)
    {}

    engine::engine(const engine &)
    {}

    si::engine::~engine()
    {}

    void si::engine::run()
    {
        d_is_running = true;

        double last_time = time::get_time();
        double unprocessed_time = 0.0;
        double frame_counter = 0.0;

        int frames = 0;

        while(d_is_running)
        {
            bool render = false;

            double start_time = time::get_time();
            double passed_time = start_time - last_time;

            last_time += passed_time;
            unprocessed_time += passed_time;
            frame_counter += passed_time;

            if(frame_counter > 1.0)
            {
                debug::print("Current FPS: ", frames);
                frame_counter = 0.0;
                frames = 0;
            }

            while(unprocessed_time > d_frame_time)
            {
                render = true;

                time::set_time_delta(d_frame_time);

                unprocessed_time -= d_frame_time;
            }

            if(render)
            {
                frames++;
            }
        }
    }
}