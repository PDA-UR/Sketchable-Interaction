

#include <sigrun/log/Log.hpp>
#include "Context.hpp"

std::unique_ptr<Capability> Context::upcm = std::make_unique<Capability>();
std::unique_ptr<RegionManager> Context::uprm = std::make_unique<RegionManager>(RegionManager());

int Context::s_width = 0;
int Context::s_height = 0;

Context::~Context()
{
    INFO("Destroying Context...", LOG_CONSOLE);
    delete p_render_thread;
    p_render_thread = nullptr;

    p_renderer = nullptr;
    INFO("Destroyed Context", LOG_CONSOLE);
}

Context::Context(int width, int height, const std::unordered_map<std::string, std::unique_ptr<bp::object>>& plugins)
{SIOBJECT("Context")
    for(const auto& item: plugins)
        upcm->add_capabilities(*item.second);

    s_width = width;
    s_height = height;
}

void Context::begin(IRenderEngine* ire)
{
    p_renderer = ire;

    p_render_thread = new std::thread([&]()
    {
        p_renderer->start(s_width, s_height);
    });

    INFO("Starting rendering thread.", LOG_CONSOLE);
    p_render_thread->join();
    INFO("Exitted threads", LOG_CONSOLE);
    INFO("Closing context...", LOG_CONSOLE);
}

Capability* Context::capability_manager()
{
    return upcm.get();
}

RegionManager *Context::region_manager()
{
    return uprm.get();
}

void Context::update()
{

}

int Context::width()
{
    return 0;
}

int Context::height()
{
    return 0;
}

void Context::enable(int what)
{

}

void Context::disable(int what)
{

}
