

#include <sigrun/log/Log.hpp>
#include "Context.hpp"

Context* Context::self = nullptr;

Context::~Context()
{
    INFO("Destroying Context...");
    delete p_render_thread;
    p_render_thread = nullptr;

    p_renderer = nullptr;
    INFO("Destroyed Context");
}

Context::Context(int width, int height, const std::unordered_map<std::string, std::unique_ptr<bp::object>>& plugins)
{SIOBJECT
    upcm = std::make_unique<Capability>();
    uprm = std::make_unique<RegionManager>(RegionManager());

    self = this;

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

    INFO("Starting rendering thread.");
    p_render_thread->join();
    INFO("Exitted threads");
    INFO("Closing context...");
}

Capability* Context::capability_manager()
{
    return upcm.get();
}

RegionManager* Context::region_manager()
{
    return uprm.get();
}

Context* Context::SIContext()
{
    return self;
}

void Context::update()
{
    self = this;
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
