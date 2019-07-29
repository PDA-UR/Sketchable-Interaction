

#include "Context.hpp"
#include "context/managers/LayerManager.hpp"
#include "debug/Print.hpp"

LayerManager* Context::sp_lm = new LayerManager();
PluginManager* Context::sp_pm = new PluginManager();
RegionManager* Context::sp_rm = new RegionManager();
int Context::s_width = 0;
int Context::s_height = 0;

Context::Context(int width, int height, const std::vector<bp::object>& plugins)
{
    s_width = 0;
    s_height = 0;

    Context::sp_lm->add_layer();
    Context::sp_pm->add_plugins(plugins);
}

Context::~Context()
{
    delete Context::sp_pm;
    Context::sp_pm = nullptr;

    delete sp_lm;
    Context::sp_lm = nullptr;
}

void Context::begin()
{

}

void Context::end()
{

}

int Context::width()
{
    return s_width;
}

void Context::set_width(int width)
{
    s_width = width;
}

int Context::height()
{
    return s_height;
}

void Context::set_height(int height)
{
    s_height = height;
}

LayerManager* Context::layer_manager()
{
    return sp_lm;
}

PluginManager* Context::plugin_manager()
{
    return sp_pm;
}
