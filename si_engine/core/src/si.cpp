//
// Created by juergen on 21/01/19.
//

#include "../include/si.h"
#include "../include/engine.h"
#include "../include/debug.h"
#include <QApplication>
#include <csignal>
#include <si.h>


namespace si
{
    /* Start of SI class */

    SI::SI(int argc, char **argv, bool test) : p_qapp(new QApplication(argc, argv))
    {
        signal(SIGABRT, signal_handler);
        signal(SIGFPE, signal_handler);
        signal(SIGILL, signal_handler);
        signal(SIGINT, signal_handler);
        signal(SIGTERM, signal_handler);

        Engine::__instance__()->start(test);
    }

    int SI::run()
    {
        connect(p_qapp, &QApplication::aboutToQuit, this, &SI::stop);
        return p_qapp->exec();
    }

    int SI::quit()
    {
        stop();

        return 0;
    }

    void SI::add_region(void *r)
    {
        if(r)
            Engine::__instance__()->add_region_template(reinterpret_cast<region *>(r));
        else
            throw std::runtime_error("Passed argument r is nullptr");
    }

    void SI::signal_handler(int signum)
    {
        debug::print("Interrupt Signal (", signum, ") received.");

        QApplication::instance()->exit(signum);
    }

    void SI::stop()
    {
        Engine::__instance__()->stop();
    }

    /* End of SI class */

    void *si_create_instance(int argc, char **argv)
    {
        return new(std::nothrow) SI(argc, argv);
    }

    void si_delete_instance(void *instance)
    {
        SI *ref = reinterpret_cast<SI *>(instance);
        instance = nullptr;

        ref->quit();

        delete ref;
        ref = nullptr;
    }

    int si_run(void *instance)
    {
        try
        {
            SI *ref = reinterpret_cast<SI *>(instance);
            return ref->run();
        }
        catch (std::exception e)
        {
            std::__throw_runtime_error(e.what());
        }

        return 0;
    }

    void si_add_region(void *instance, void *region)
    {
        if(region == nullptr)
            std::__throw_runtime_error("Given region argument is a nullptr");

        try
        {
            SI *ref = reinterpret_cast<SI *>(instance);

            // construct actual region object type?

            ref->add_region(region);
        }
        catch  (std::exception e)
        {
            std::__throw_runtime_error(e.what());
        }
    }

    void *si_region_create_instance(region_callback rce, region_callback rcc, region_callback rcl, region_callback rocc, region_callback rdc)
    {
        if(!rce)
            std::__throw_runtime_error("callback function on_region_enter not set");

        if(!rcc)
            std::__throw_runtime_error("callback function on_region_continuous not set");

        if(!rcl)
            std::__throw_runtime_error("callback function on_region_leave not set");

        if(!rocc)
            std::__throw_runtime_error("callback function on_region_create not set");

        if(!rdc)
            std::__throw_runtime_error("callback function on_region_destroy not set");

        return new(std::nothrow) si::region(rce, rcc, rcl, rocc, rdc);
    }

    void si_region_set_blueprint(void *instance, void *r, int flag)
    {
        if(r == nullptr)
            std::__throw_runtime_error("Given region argument is a nullptr");

        try
        {
            SI *ref = reinterpret_cast<SI *>(instance);
            region *region_ref = reinterpret_cast<region *>(r);

            region_ref->set_blueprint(flag > 0);

            if (flag > 0)
                flag = flag;
                // add to engine blueprint list on addition
            //else
                // do not add to engine blueprint list on addition
        }
        catch  (std::exception e)
        {
            std::__throw_runtime_error(e.what());
        }
    }

    void si_region_set_region_color(void *instance, void *reg, int r, int g, int b, int a)
    {
        if(reg == nullptr)
            std::__throw_runtime_error("Given region argument is a nullptr");

        try
        {
            SI *ref = reinterpret_cast<SI *>(instance);
            region *ref_region = reinterpret_cast<region *>(reg);

            ref_region->set_region_color(r, g, b, a);
        }
        catch  (std::exception e)
        {
            std::__throw_runtime_error(e.what());
        }
    }

    void si_region_set_as_main_canvas(void *instance, void *reg)
    {
        if(reg == nullptr)
            std::__throw_runtime_error("Given region argument is a nullptr");

        try
        {
            SI *ref = reinterpret_cast<SI *>(instance);
            region *ref_region = reinterpret_cast<region *>(reg);

            //ref->override_main_canvas(ref_region);
        }
        catch  (std::exception e)
        {
            std::__throw_runtime_error(e.what());
        }
    }

    void si_region_set_fullscreen(void *instance, void *reg, int flag)
    {
        if(reg == nullptr)
            std::__throw_runtime_error("Given region argument is a nullptr");

        try
        {
            SI *ref = reinterpret_cast<SI *>(instance);
            region *ref_region = reinterpret_cast<region *>(reg);

            // only applys to main canvas
            // fetch engine and set fullscreen or not based on flag
            // by default on
        }
        catch  (std::exception e)
        {
            std::__throw_runtime_error(e.what());
        }
    }

    void si_region_set_as_main_interaction_source(void *instance, void *reg, int *x_values, int *y_values, int length)
    {
        if(reg == nullptr)
            std::__throw_runtime_error("Given region argument is a nullptr");

        try
        {
            SI *ref = reinterpret_cast<SI *>(instance);
            region *ref_region = reinterpret_cast<region *>(reg);

            // probably redundant due to capability system?

            QPolygon p;

            for(int i = 0; i < length; i++)
                p << QPoint(x_values[i], y_values[i]);

            ref_region->set_shape(p);
            ref_region->set_as_main_interaction_source();
        }
        catch  (std::exception e)
        {
            std::__throw_runtime_error(e.what());
        }
    }

    void si_region_set_region_on_enter_callback(void *instance, void *reg, region_callback rc)
    {
        if(reg == nullptr)
            std::__throw_runtime_error("Given region argument is a nullptr");

        try
        {
            SI *ref = reinterpret_cast<SI *>(instance);
            region *ref_region = reinterpret_cast<region *>(reg);

            ref_region->set_enter_callback(rc);
        }
        catch  (std::exception e)
        {
            std::__throw_runtime_error(e.what());
        }
    }

    void si_region_set_region_on_continuous_callback(void *instance, void *reg, region_callback rc)
    {
        if(reg == nullptr)
            std::__throw_runtime_error("Given region argument is a nullptr");

        try
        {
            SI *ref = reinterpret_cast<SI *>(instance);
            region *ref_region = reinterpret_cast<region *>(reg);

            ref_region->set_continuous_callback(rc);
        }
        catch  (std::exception e)
        {
            std::__throw_runtime_error(e.what());
        }
    }

    void si_region_set_region_on_leave_callback(void *instance, void *reg, region_callback rc)
    {
        if(reg == nullptr)
            std::__throw_runtime_error("Given region argument is a nullptr");

        try
        {
            SI *ref = reinterpret_cast<SI *>(instance);
            region *ref_region = reinterpret_cast<region *>(reg);

            ref_region->set_leave_callback(rc);
        }
        catch  (std::exception e)
        {
            std::__throw_runtime_error(e.what());
        }
    }
}




