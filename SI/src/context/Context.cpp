

#include "Context.hpp"
#include "context/managers/LayerManager.hpp"
#include "debug/Print.hpp"

LayerManager* Context::s_lm = new LayerManager();

Context::Context(int width, int height, const std::vector<bp::object>& plugins)
    : d_width(width), d_height(height), d_plugins(plugins)
{
    s_lm->add_layer();
}

Context::~Context()
{
    delete s_lm;
    s_lm = nullptr;
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

const std::vector<bp::object>& Context::plugins() const
{
    return d_plugins;
}

void Context::add_plugin(const bp::object &plugin)
{
    // do check whether it is already present later

    d_plugins.push_back(plugin);
}

LayerManager* Context::layer_manager()
{
    return s_lm;
}
