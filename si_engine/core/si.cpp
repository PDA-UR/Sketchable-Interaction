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
}




