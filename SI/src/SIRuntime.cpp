
#include <string.h>
#include <core/Core.hpp>
#include "SIRuntime.hpp"

int SI_start(const char* plugin_path)
{
    p_core = new Core();
    p_core->start(plugin_path);

    return 0;
}

int SI_pause()
{

    return 0;
}

int SI_end()
{
    p_core->stop();

    delete p_core;
    p_core = nullptr;

    return 0;
}
