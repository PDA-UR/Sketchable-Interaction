

#include "Plugin.hpp"

namespace bp = boost::python;

void Plugin::set_pyplugin(const bp::object &plugin)
{
    d_pyplugin = plugin;
}

SIPlugin::SIPlugin(const bp::object &pyplugin)
{
    set_pyplugin(pyplugin);
}

int SIPlugin::on_enter()
{
    d_pyplugin.attr("on_enter")();

    return 0;
}

int SIPlugin::on_continuous()
{
    d_pyplugin.attr("on_continuous")();

    return 0;
}

int SIPlugin::on_leave()
{
    d_pyplugin.attr("on_leave")();

    return 0;
}

std::ostream &operator<<(std::ostream &os, const SIPlugin &plugin)
{
    return os << "Python3 Plugin: " << bp::extract<std::string>(bp::str(plugin.d_pyplugin))();
}

class PyPlugin : public Plugin, public bp::wrapper<Plugin>
{
    int on_enter() override
    {
        get_override("on_enter")();

        return 0;
    }

    int on_continuous() override
    {
        get_override("on_continuous")();

        return 0;
    }

    int on_leave() override
    {
        get_override("on_leave")();

        return 0;
    }
};

BOOST_PYTHON_MODULE(libPySI)
{
    bp::class_<PyPlugin, boost::noncopyable>("PySiRegion", bp::init<>())
            .def("on_enter", &Plugin::on_enter)
            .def("on_continuous", &Plugin::on_continuous)
            .def("on_leave", &Plugin::on_leave)
            ;
}