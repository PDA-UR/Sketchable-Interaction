

#include <sigrun/log/Log.hpp>
#include "Context.hpp"
#include <QApplication>
#include <sigrun/rendering/IRenderEngine.hpp>
#include <boost/python.hpp>

namespace bp = boost::python;

Context* Context::self = nullptr;

Context::~Context()
{
    INFO("Destroying Context...");

    delete d_render_worker;
    d_render_worker = nullptr;

    INFO("Destroyed Context");
}

Context::Context(int width, int height, const std::unordered_map<std::string, std::unique_ptr<bp::object>>& plugins)
{SIOBJECT(SIGRUN)
    upcm = std::make_unique<Capability>();
    uprm = std::make_unique<RegionManager>();
    uplm = std::make_unique<LinkingManager>();

    self = this;

    for(const auto& item: plugins)
        upcm->add_capabilities(*item.second);

    s_width = width;
    s_height = height;
    std::vector<glm::vec3> contour1 {glm::vec3(10, 10, 1), glm::vec3(10, 60, 1), glm::vec3(60, 60, 1), glm::vec3(60, 10, 1)};
    std::vector<glm::vec3> contour2 {glm::vec3(80, 10, 1), glm::vec3(80, 60, 1), glm::vec3(70 + 60, 60, 1), glm::vec3(70 + 60, 10, 1)};

    uprm->add_region(contour1, std::make_shared<bp::object>(*plugins.begin()->second), 0);
    uprm->add_region(contour2, std::make_shared<bp::object>(*plugins.begin()->second), 1);
}

void Context::begin(IRenderEngine* ire, int argc, char** argv)
{
    INFO("Creating Qt5 Application...");
    QApplication qapp(argc, argv);
    INFO("Qt5 Application created!");

    d_render_worker = new RenderWorker(s_width, s_height, ire);

    INFO("Setting Up Rendering Background Worker...");
    initialize_rendering_worker();
    INFO("Finished Setting Up Rendering Background Worker!");

    INFO("Starting Rendering Thread");
    d_render_thread.start();
    INFO("Rendering Thread Started!");

    uplm->add_link(uprm->regions()[0], "__position__", uprm->regions()[1],"__position__", ILink::LINK_TYPE::UD);

    const bp::list& py_list = bp::extract<bp::list>(uprm->regions()[0]->effect().attr("cap_link_emit")["__position__"]());

    Q_EMIT uprm->regions()[0]->LINK_SIGNAL(UUID::uuid(), "__position__", "__position__", py_list);

    qapp.exec();
    INFO("QT5 Application terminated!");
}

void Context::initialize_rendering_worker()
{
    d_render_worker->moveToThread(&d_render_thread);

    connect(&d_render_thread, &QThread::started, d_render_worker, &RenderWorker::render);
    connect(d_render_worker, &RenderWorker::finished, &d_render_thread, &QThread::quit);
    connect(d_render_worker, &RenderWorker::finished, d_render_worker, &RenderWorker::deleteLater);
    connect(&d_render_thread, &QThread::finished, &d_render_thread, &QThread::deleteLater);
    connect(QApplication::instance(), &QApplication::aboutToQuit, &d_render_thread, &QThread::quit);
    connect(&d_render_thread, &QThread::finished, QApplication::instance(), &QApplication::closeAllWindows);

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
