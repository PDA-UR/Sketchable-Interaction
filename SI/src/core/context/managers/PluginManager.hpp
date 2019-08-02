

#ifndef SI_PLUGINMANAGER_HPP
#define SI_PLUGINMANAGER_HPP

#include <core/pysi_core/Plugin.hpp>
#include <boost/python.hpp>
#include <core/runtime_core/region/Region.hpp>

namespace bp = boost::python;

// watchdog for updating pysi list by listening to pysi folder changes
// at runtime_core insertion of new plugins

class PluginManager
{
public:
    const std::vector<Region>& plugins() const;
    const int num_plugins() const;

private:
    PluginManager();
    ~PluginManager();

    void add_plugins(const std::vector<bp::object>& plugins);

    void destroy();

    std::vector<Region> d_plugins;

    friend class Context;
    friend class SiPluginManagerTest;
};


#endif //SI_PLUGINMANAGER_HPP
