//
// Created by juergen on 25/01/19.
//

#include "plugin.h"

namespace si
{
    plugin::plugin(const char *type,
                   plugin_callback orec,
                   plugin_callback orcc,
                   plugin_callback orlc,
                   plugin_callback occ,
                   plugin_callback odc) :
            d_type(std::string(type)),
            on_enter(orec),
            on_continuous(orcc),
            on_leave(orlc),
            on_create(occ),
            on_destroy(odc)
    {}

    void *si_plugin_create_instance(const char *type, plugin_callback orec, plugin_callback orcc,
                                    plugin_callback orlc, plugin_callback occ, plugin_callback odc)
    {
        return new(std::nothrow) plugin(type, orec, orcc, orlc, occ, odc);
    }

    void si_plugin_delete_instance(void *instance)
    {
        delete instance;
    }
}

