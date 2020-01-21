

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
    INFO("Destroyed Context");
}

Context::Context(int width, int height, const std::unordered_map<std::string, std::unique_ptr<bp::object>>& plugins)
{SIGRUN
    self = this;
    s_width = width;
    s_height = height;

    uprm = std::make_unique<RegionManager>();


    for(const auto& item: plugins)
        upcm->add_capabilities(*item.second);

    std::vector<glm::vec3> contour1 {glm::vec3(10, 10, 1), glm::vec3(10, 60, 1), glm::vec3(60, 60, 1), glm::vec3(60, 10, 1)};
    std::vector<glm::vec3> contour2 {glm::vec3(80, 10, 1), glm::vec3(80, 60, 1), glm::vec3(70 + 60, 60, 1), glm::vec3(70 + 60, 10, 1)};

    uprm->add_region(contour1, std::make_shared<bp::object>(*plugins.begin()->second), 0);
    uprm->add_region(contour2, std::make_shared<bp::object>(*plugins.begin()->second), 1);
}

void Context::begin(IRenderEngine* ire, int argc, char** argv)
{
    INFO("Creating Qt5 Application...");
    QApplication d_app(argc, argv);
    INFO("Qt5 Application created!");

    d_ire = ire;

    uplm = std::make_unique<LinkingManager>();
    upim = std::make_unique<InputManager>();
    upcm = std::make_unique<Capability>();

    d_app.installEventFilter(upim.get());

    d_ire->start(s_width, s_height);

    d_app.exec();
    INFO("QT5 Application terminated!");
}

void Context::end()
{
    d_ire->stop();
}

Capability* Context::capability_manager()
{
    return upcm.get();
}

RegionManager* Context::region_manager()
{
    return uprm.get();
}

InputManager* Context::input_manager()
{
    return upim.get();
}

Context* Context::SIContext()
{
    return self;
}

void Context::update()
{
    self = this;

    upim->update();
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
