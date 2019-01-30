//
// Created by juergen on 21/01/19.
//

#include "si.h"
#include "si_engine/engine.h"
#include "si_debug/debug.h"
#include <QApplication>

namespace si
{
    /* Start of SI class */

    SI::SI(int argc, char **argv) : p_qapp(new QApplication(argc, argv))
    {
        Engine::__instance__()->start();
    }

    int SI::run()
    {
        connect(p_qapp, &QApplication::aboutToQuit, this, &SI::stop);
        return p_qapp->exec();
    }

    void SI::add_region(void *r)
    {
        Engine::__instance__()->add_region_template(reinterpret_cast<region *>(r));
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
            return -1;
        }
    }

    void si_add_region(void *instance, void *region)
    {
        try{
            SI *ref = reinterpret_cast<SI *>(instance);

            // construct actual region object type?

            ref->add_region(region);
        }
        catch  (std::exception e)
        {
            return;
        }

    }

    void *si_region_create_instance(region_callback rce, region_callback rcc, region_callback rcl)
    {
        return new(std::nothrow) si::region(rce, rcc, rcl);
    }
}




