

#include "Context.hpp"
#include "core/context/managers/LayerManager.hpp"
#include "core/debug/Print.hpp"

LayerManager* Context::sp_lm = new LayerManager();
PluginManager* Context::sp_pm = new PluginManager();
RegionManager* Context::sp_rm = new RegionManager();

int Context::s_width = 0;
int Context::s_height = 0;

Context::Context(int width, int height, const std::vector<bp::object>& plugins)
{
    s_width = width;
    s_height = height;

    Context::sp_lm->add_layer();
    Context::sp_pm->add_plugins(plugins);
//
//    std::vector<Vector2<float>> contour =
//    {
//        Vector2<float>(100, 100),
//        Vector2<float>(100, 600),
//        Vector2<float>(350, 375),
//        Vector2<float>(600, 600),
//        Vector2<float>(600, 100),
//        Vector2<float>(350, 325),
//    };
//
//    Region r;
//    r.set_effect(plugins[0]);
//    r.set_shape(contour);
//    Context::sp_rm->add_region(r);

    Print::print("Context Constructed");
}

Context::~Context()
{
    delete Context::sp_pm;
    Context::sp_pm = nullptr;

    delete sp_lm;
    Context::sp_lm = nullptr;

    delete sp_rm;
    Context::sp_rm = nullptr;

    delete p_renderer;
    p_renderer = nullptr;

    delete p_render_thread;
    p_render_thread = nullptr;

    Print::print("Context Destroyed");
}

void Context::begin(IRenderEngine* ire)
{
    p_renderer = ire;

    p_render_thread = new std::thread([&]()
    {
        p_renderer->start(s_width, s_height);
        Print::print("terminate");
    });

    p_render_thread->join();
}

void Context::end()
{
    p_renderer->stop();
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

RegionManager* Context::region_manager()
{
    return sp_rm;
}