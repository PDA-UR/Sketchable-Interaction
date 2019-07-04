
#include <cstring>
#include "SI.hpp"

int SI_initialize(const char *plugin_path)
{
    if(plugin_path == nullptr)
    {
        return 1;
    }

    if(!strcmp(plugin_path, ""))
    {
        return 2;
    }

    return 0;
}

int SI_start()
{

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
