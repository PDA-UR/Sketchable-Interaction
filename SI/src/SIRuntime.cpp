
#include <string.h>
#include <core/Core.hpp>
#include "SIRuntime.hpp"

int SI_start(const char* plugin_path)
{
    Core c = Core();
    c.start(plugin_path);

    return 0;
}

int SI_pause()
{

    return 0;
}

int SI_end()
{

    return 0;
}
