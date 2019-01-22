//
// Created by juergen on 21/01/19.
//

#include "si.h"
#include "si_engine/engine.h"
#include "si_debug/debug.h"

namespace si
{
    int run()
    {
        engine *engine = engine::__instance__();
        engine->start();

        return 0;
    }

    int register_plugin(void *plugin)
    {
        debug::print(*(int *) plugin);

        return 0;
    }
}




