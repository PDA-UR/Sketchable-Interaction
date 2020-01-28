

#include <sigrun/log/Log.hpp>
#include "Context.hpp"
#include <QApplication>
#include <sigrun/rendering/IRenderEngine.hpp>
#include <boost/python.hpp>
#include <pysi/SuperEffect.hpp>

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

    frame_num = 0;

    uprm = std::make_unique<RegionManager>();
    uplm = std::make_unique<LinkingManager>();
    upim = std::make_unique<InputManager>();
    upcm = std::make_unique<Capability>();
    uprcm = std::make_unique<CollisionManager>();

    for(const auto& item: plugins)
        upcm->add_capabilities(*item.second);

    // add a canvas region
    for(auto& [key, value]: plugins)
    {
        if(bp::extract<int>(value->attr("region_type")) == PySIEffect::EffectType::SI_CANVAS)
        {
            std::vector<glm::vec3> canvas_contour {glm::vec3(0, 0, 1), glm::vec3(0, height, 1), glm::vec3(width, height, 1), glm::vec3(width, 0, 1) };
            uprm->add_region(canvas_contour, std::make_shared<bp::object>(*value), 0);
        }
    }

    // add a mouse region

    for(auto& [key, value]: plugins)
    {
        if(bp::extract<int>(value->attr("region_type")) == PySIEffect::EffectType::SI_MOUSE_CURSOR)
        {
            std::vector<glm::vec3> mouse_contour {glm::vec3(0, 0, 1), glm::vec3(0, 16, 1), glm::vec3(12, 16, 1), glm::vec3(12, 0, 1) };
            uprm->add_region(mouse_contour, std::make_shared<bp::object>(*value), 0);
            uplm->add_link_to_object(uprm->regions().back(), ExternalObject::ExternalObjectType::MOUSE);
        }
    }
    // ------------------------------
}

void Context::begin(IRenderEngine* ire, int argc, char** argv)
{
    INFO("Creating Qt5 Application...");
    QApplication d_app(argc, argv);
    INFO("Qt5 Application created!");

//    d_app.setOverrideCursor(Qt::BlankCursor);

    d_ire = ire;

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

CollisionManager *Context::collision_manager()
{
    return uprcm.get();
}

Context* Context::SIContext()
{
    return self;
}

/*
 * API DESIGN:
 * if an effect manipulates the object itself use properties
 * if an effect does something else use functions
 *
 * Example:
 *
 * Use properties for position, scale, rotation, etc.
 * Use functions for setting points of sketching, data content, etc.
 */
void Context::update()
{
    upim->update();
    uprm->update();

    uprcm->collide(uprm->regions());

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
