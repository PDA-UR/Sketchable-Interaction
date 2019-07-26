
#include "PluginManager.hpp"

const std::vector<Region>& PluginManager::plugins() const
{
    return d_plugins;
}

void PluginManager::add_plugins(const std::vector<bp::object> &plugins)
{
    for(auto& p : plugins)
    {
        Region r;
        r.set_effect(p);

        d_plugins.push_back(r);
    }
}

const int PluginManager::num_plugins() const
{
    return d_plugins.size();
}

void PluginManager::destroy()
{
    d_plugins.clear();
}

PluginManager::PluginManager()
= default;

PluginManager::~PluginManager()
{
    destroy();
}
