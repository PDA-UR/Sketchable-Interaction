//
// Created by juergen on 25/01/19.
//

#ifndef CORE_PLUGIN_H
#define CORE_PLUGIN_H


#include <QtWidgets/QWidget>

namespace si
{   extern "C"
    {
        typedef int (*plugin_callback)(long);
    }

    class plugin
    {
    public:
        plugin(const char *type,
               plugin_callback orec,
               plugin_callback orcc,
               plugin_callback orlc,
               plugin_callback occ,
               plugin_callback odc);

        plugin_callback on_enter;
        plugin_callback on_continuous;
        plugin_callback on_leave;
        plugin_callback on_create;
        plugin_callback on_destroy;

        std::string const &type() const
        {
            return d_type;
        }

    private:
        std::string d_type;
    };

    extern "C"
    {
        void *si_plugin_create_instance(const char *type, plugin_callback orec, plugin_callback orcc, plugin_callback orlc, plugin_callback occ, plugin_callback odc);
        void si_plugin_delete_instance(void * instance);
    }
}


#endif //CORE_PLUGIN_H
