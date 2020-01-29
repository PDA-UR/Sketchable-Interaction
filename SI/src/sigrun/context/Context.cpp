

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

    uprm = std::unique_ptr<RegionManager>(new RegionManager);
    uplm = std::unique_ptr<LinkingManager>(new LinkingManager);
    upim = std::unique_ptr<InputManager>(new InputManager);
    upcm = std::unique_ptr<Capability>(new Capability);
    uprcm = std::unique_ptr<CollisionManager>(new CollisionManager);

    for(const auto& item: plugins)
        upcm->add_capabilities(*item.second);

    // add a canvas region
    for(auto& [key, value]: plugins)
    {
        if (bp::extract<int>(value->attr("region_type")) == PySIEffect::EffectType::SI_CANVAS)
        {
            std::vector<glm::vec3> canvas_contour{glm::vec3(0, 0, 1), glm::vec3(0, height, 1), glm::vec3(width, height, 1), glm::vec3(width, 0, 1)};
            uprm->add_region(canvas_contour, std::shared_ptr<bp::object>(new bp::object(*value)), 0);
            d_canvas_uuid = uprm->regions().back()->uuid();
        }
    }

    for(auto& [key, value]: plugins)
    {
        switch (bp::extract<int>(value->attr("region_type")))
        {
            case PySIEffect::EffectType::SI_CANVAS: break;
            case PySIEffect::EffectType::SI_MOUSE_CURSOR:
            {
                std::vector<glm::vec3> mouse_contour {glm::vec3(0, 0, 1), glm::vec3(0, 16, 1), glm::vec3(12, 16, 1), glm::vec3(12, 0, 1) };
                uprm->add_region(mouse_contour, std::shared_ptr<bp::object>(new bp::object(*value)), 0);
                uplm->add_link_to_object(uprm->regions().back(), ExternalObject::ExternalObjectType::MOUSE);
                d_mouse_uuid = uprm->regions().back()->uuid();
            }
            break;

            default:
                d_available_plugins.push_back(*value);
            break;
        }
    }

    INFO("Plugins available for drawing: " + std::to_string(d_available_plugins.size()));

    d_selected_effects_by_id[d_mouse_uuid] = d_available_plugins.back();
    const std::string& selected = bp::extract<std::string>(d_selected_effects_by_id[d_mouse_uuid].attr("name"));

    INFO("Mouse Cursor: " + d_mouse_uuid + " set to " + selected);
}

void Context::begin(IRenderEngine* ire, int argc, char** argv)
{
    INFO("Creating Qt5 Application...");
    QApplication d_app(argc, argv);
    INFO("Qt5 Application created!");

    d_app.setOverrideCursor(Qt::BlankCursor);

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
    uprcm->collide(uprm->regions());
    uprm->update();

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

void Context::register_new_region(const std::vector<glm::vec3>& contour, const std::string& uuid)
{
    uprm->add_region(contour, std::make_shared<bp::object>(d_selected_effects_by_id[uuid]), 0);
}

void Context::update_linking_relations(const std::vector<std::shared_ptr<LinkRelation>>& relations)
{
    for(auto& lr: relations)
    {
        int index_sender = -1;
        int index_recv = -1;

        for(int i = 0; i < uprm->regions().size(); ++i)
        {
            auto& region = uprm->regions()[i];

            if(region->uuid() == lr->sender)
                index_sender = i;

            if(region->uuid() == lr->recv)
                index_recv = i;
        }


        // handling to properly add and remove links based on list of current links
        if(index_sender != -1 && index_recv != -1)
        {
            if(std::find(d_links_in_ctx.begin(), d_links_in_ctx.end(), lr) == d_links_in_ctx.end())
            {
                DEBUG("NEW");
                d_links_in_ctx.push_back(lr);
                uplm->add_link(uprm->regions()[index_sender], lr->sender_attrib, uprm->regions()[index_recv], lr->recv_attrib, ILink::LINK_TYPE::UD);
            }
            else
            {
                DEBUG("OLD");
            }
        }
    }
}
