//
// Created by juergen on 21/01/19.
//

#include "../include/si.h"
#include "../include/engine.h"
#include "../include/debug.h"
#include <QApplication>

namespace si
{
    /* Start of SI class */

    SI::SI(int argc, char **argv, bool test) : p_qapp(new QApplication(argc, argv))
    {
        Engine::__instance__()->start(test);
    }

    int SI::run()
    {
        connect(p_qapp, &QApplication::aboutToQuit, this, &SI::stop);
        return p_qapp->exec();
    }

    void SI::add_region(void *r)
    {
        if(r)
            Engine::__instance__()->add_region_template(reinterpret_cast<region *>(r));
        else
            throw std::runtime_error("Passed argument r is nullptr");
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
        delete instance;
        instance = nullptr;
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

    void *si_region_create_instance(region_callback rce, region_callback rcc, region_callback rcl)
    {
        if(!rce)
            std::__throw_runtime_error("callback function on_region_enter not set");

        if(!rcc)
            std::__throw_runtime_error("callback function on_region_continuous not set");

        if(!rcl)
            std::__throw_runtime_error("callback function on_region_leave not set");

        return new(std::nothrow) si::region(rce, rcc, rcl);
    }
}




