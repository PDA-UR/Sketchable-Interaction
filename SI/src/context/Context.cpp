

#include "Context.hpp"
#include "context/managers/LayerManager.hpp"
#include "debug/Print.hpp"

LayerManager* Context::s_lm = new LayerManager();
PluginManager* Context::s_pm = new PluginManager();

Context::Context(int width, int height, const std::vector<bp::object>& plugins)
    : d_width(width), d_height(height)
{
    Context::s_lm->add_layer();
    Context::s_pm->add_plugins(plugins);
}

Context::~Context()
{
    delete Context::s_pm;
    Context::s_pm = nullptr;

    delete s_lm;
    Context::s_lm = nullptr;
}

void Context::begin()
{

}

void Context::end()
{

}

int Context::width() const
{
    return d_width;
}

void Context::set_width(int width)
{
    d_width = width;
}

int Context::height() const
{
    return d_height;
}

void Context::set_height(int height)
{
    d_height = height;
}

LayerManager* Context::layer_manager()
{
    return s_lm;
}

PluginManager* Context::plugin_manager()
{
    return s_pm;
}
