

#include <sigrun/log/Log.hpp>
#include "Context.hpp"
#include <QApplication>
#include <QMainWindow>
#include <sigrun/rendering/IRenderEngine.hpp>
#include <boost/python.hpp>
#include <pysi/PySIEffect.hpp>
#include <sigrun/util/Util.hpp>

namespace bp = boost::python;

Context* Context::self = nullptr;

Context::~Context()
{
    INFO("Destroying Context...");
    INFO("Destroyed Context");
}

Context::Context()
{
    self = this;

    uprm = std::unique_ptr<RegionManager>(new RegionManager);
    uplm = std::unique_ptr<LinkingManager>(new LinkingManager);
    upim = std::unique_ptr<InputManager>(new InputManager);
    upcm = std::unique_ptr<Capability>(new Capability);
    uprcm = std::unique_ptr<CollisionManager>(new CollisionManager);
    upfs = std::unique_ptr<FileSystem>(new FileSystem);
    upeam = std::unique_ptr<ExternalApplicationManager>(new ExternalApplicationManager);
}

void Context::add_startup_regions(const std::unordered_map<std::string, std::unique_ptr<bp::object>>& plugins)
{
    add_canvas_region(plugins);

    for(auto& [key, value]: plugins)
    {
        if(!value->is_none())
        {
            DEBUG(key);

            HANDLE_PYTHON_CALL(
                switch (bp::extract<int>(value->attr("region_type")))
                {
                    case SI_TYPE_CANVAS: break;
                    case SI_TYPE_MOUSE_CURSOR:
                        add_cursor_regions(value);
                        break;
                    case SI_TYPE_NOTIFICATION:
                    {
                        const int& width = bp::extract<int>(value->attr("width"));
                        const int& height = bp::extract<int>(value->attr("height"));

                        uint32_t x = s_width / 2 - width / 2;

                        std::vector<glm::vec3> contour{glm::vec3(x, 1, 1),
                                                      glm::vec3(x, height, 1),
                                                      glm::vec3(x + width, height - 1, 1),
                                                      glm::vec3(x + width, 1, 1)};
                        uprm->add_region(contour, *value, 0, bp::dict());
                        d_notification_uuid = uprm->regions().back()->uuid();
                    }
                    break;
                    case SI_TYPE_DIRECTORY:
                        add_directory_region(value);
                    default:
                    {
                        d_available_plugins[key] = *value;
                    }
                        break;
                }
            )
        }
    }
}

void Context::add_canvas_region(const std::unordered_map<std::string, std::unique_ptr<bp::object>>& plugins)
{
    for(auto& [key, value]: plugins)
    {
        if (!value->is_none())
        {
            HANDLE_PYTHON_CALL(
                if (bp::extract<uint32_t>(value->attr("region_type")) == SI_TYPE_CANVAS)
                {
                    std::vector<glm::vec3> canvas_contour{glm::vec3(1, 1, 1), glm::vec3(1, s_height - 1, 1),
                                                          glm::vec3(s_width - 1, s_height - 1, 1),
                                                          glm::vec3(s_width - 1, 1, 1)};
                    uprm->add_region(canvas_contour, *value, 0, bp::dict());
                    d_canvas_uuid = uprm->regions().back()->uuid();
                }
            )
        }
    }
}

void Context::add_cursor_regions(const std::unique_ptr<bp::object>& cursor_effect)
{
    uint32_t width_mouse_cursor = bp::extract<uint32_t>(cursor_effect->attr("width"));
    uint32_t height_mouse_cursor = bp::extract<uint32_t>(cursor_effect->attr("height"));

    std::vector<glm::vec3> mouse_contour {glm::vec3(0, 0, 1), glm::vec3(0, height_mouse_cursor, 1), glm::vec3(width_mouse_cursor, height_mouse_cursor, 1), glm::vec3(width_mouse_cursor, 0, 1) };
    uprm->add_region(mouse_contour, *cursor_effect, 0, bp::dict());
    d_mouse_uuid = uprm->regions().back()->uuid();

    upim->external_objects()[d_mouse_uuid] = std::make_shared<ExternalObject>(ExternalObject::ExternalObjectType::MOUSE);
    uplm->add_link_to_object(uprm->regions().back(), ExternalObject::ExternalObjectType::MOUSE);

    INFO("Plugin available for drawing");
}

void Context::add_directory_region(const std::unique_ptr<bp::object>& directory_effect)
{
    const std::string& cwd = upfs->cwd();
    const std::vector<std::string> children_paths = upfs->cwd_contents_paths(cwd);

    INFO("Creating Region for " + cwd);

    uint32_t width_directory = bp::extract<uint32_t>(directory_effect->attr("width"));
    uint32_t height_directory = bp::extract<uint32_t>(directory_effect->attr("height"));

    std::vector<glm::vec3> dir_contour {glm::vec3(0, 0, 1), glm::vec3(0, height_directory, 1), glm::vec3(width_directory, height_directory, 1), glm::vec3(width_directory, 0, 1) };

    bp::dict kwargs;

    kwargs["cwd"] = cwd;
    kwargs["children"] = children_paths;
    kwargs["is_child"] = false;

    uprm->add_region(dir_contour, *directory_effect, 0, kwargs);

    INFO("Region for " + cwd + " created");
}

void Context::begin(const std::unordered_map<std::string, std::unique_ptr<bp::object>>& plugins, IRenderEngine* ire, int argc, char** argv)
{
    if(ire)
    {
        d_ire = ire;

        INFO("Creating Qt5 Application...");
        QApplication d_app(argc, argv);
        INFO("Qt5 Application created!");

//        d_app.setOverrideCursor(Qt::BlankCursor);

        d_app.installEventFilter(upim.get());

        d_ire->start(s_width, s_height);

        add_startup_regions(plugins);

        if(!d_available_plugins.empty())
        {
            DEBUG("Test case with further effect type");

            d_selected_effects_by_id[d_mouse_uuid] = d_available_plugins["Tag"];
            const std::string& selected = bp::extract<std::string>(d_selected_effects_by_id[d_mouse_uuid].attr("name"));
            DEBUG("Mouse Cursor: " + d_mouse_uuid + " set to " + selected);
        }

        d_app.exec();
        INFO("QT5 Application terminated!");
    }
}

void Context::end()
{
    if(d_ire)
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

LinkingManager* Context::linking_manager()
{
    return uplm.get();
}

ExternalApplicationManager* Context::external_application_manager()
{
    return upeam.get();
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
    if(upim->is_key_pressed(SI_KEY_A))
    {
        if(test_help == "Tag")
            test_help = "Deletion";
        else if(test_help == "Deletion")
            test_help = "OpenEntry";
        else
            test_help = "Tag";

        d_selected_effects_by_id[d_mouse_uuid] = d_available_plugins[test_help];
        const std::string& selected = bp::extract<std::string>(d_selected_effects_by_id[d_mouse_uuid].attr("name"));

        for(auto& region: uprm->regions())
        {
            if(region->type() == SI_TYPE_NOTIFICATION)
            {
                region->raw_effect().attr("update_message")("Mouse Cursor set to " + selected);
                break;
            }
        }
    }

    upim->update();
    uprcm->collide(uprm->regions());
    uprm->update();
}

uint32_t Context::width()
{
    return s_width;
}

uint32_t Context::height()
{
    return s_height;
}

void Context::enable(uint32_t what)
{

}

void Context::disable(uint32_t what)
{

}

void Context::register_new_region(const std::vector<glm::vec3>& contour, const std::string& uuid)
{
    if(contour.size() > 10)
    {
        std::shared_ptr<Region> t(nullptr);
        uprm->query_region_insertion(contour, d_selected_effects_by_id[uuid], t);
    }
}

void Context::update_linking_relations(const std::vector<LinkRelation>& relations, const std::string& source)
{
    if (relations.empty())
        remove_all_source_linking_relations(source);
    else
    {
        remove_linking_relations(relations, source);
        create_linking_relations(relations, source);
    }
}

void Context::remove_all_partaking_linking_relations(const std::string &source)
{
    remove_all_source_linking_relations(source);

    std::vector<uint32_t> indices;
    indices.reserve(uplm->links().size());

    uint32_t i = 0;
    for(auto& link: uplm->links())
    {
        if(link->sender_a()->uuid() == source || link->receiver_b()->uuid() == source)
            indices.push_back(i);

        ++i;
    }

    uplm->remove_links_by_indices(indices);
}

void Context::remove_all_source_linking_relations(const std::string &source)
{
    if(MAP_HAS_KEY(d_links_in_ctx, source))
    {
        std::transform(std::execution::par_unseq, d_links_in_ctx[source].begin(), d_links_in_ctx[source].end(), d_links_in_ctx[source].begin(), [&](auto& i)
        {
            uplm->remove_link(i->sender_a(), i->attribute_a(), i->receiver_b(), i->attribute_b(), ILink::LINK_TYPE::UD);
            return i;
        });

        d_links_in_ctx.erase(source);
    }
}

void Context::remove_linking_relations(const std::vector<LinkRelation> &relations, const std::string &source)
{
    if (MAP_HAS_KEY(d_links_in_ctx, source))
    {
        d_links_in_ctx[source].erase(std::remove_if(std::execution::par_unseq, d_links_in_ctx[source].begin(), d_links_in_ctx[source].end(), [&](auto& link) -> bool
        {
            if(std::find_if(std::execution::par_unseq, relations.begin(), relations.end(), [&link](const LinkRelation &x)
            {
                return link->sender_a()->uuid() == x.sender &&
                       link->attribute_a() == x.sender_attrib &&
                       link->receiver_b()->uuid() == x.recv &&
                       link->attribute_b() == x.recv_attrib;
            }) == relations.end())
            {
                uplm->remove_link(link->sender_a(), link->attribute_a(), link->receiver_b(), link->attribute_b(), ILink::LINK_TYPE::UD);
                return true;
            }

            return false;

        }), d_links_in_ctx[source].end());
    }
}

void Context::create_linking_relations(const std::vector<LinkRelation> &relations, const std::string &source)
{
    if (!MAP_HAS_KEY(d_links_in_ctx, source))
    {
        d_links_in_ctx[source] = std::vector<std::shared_ptr<ILink>>();
        d_links_in_ctx[source].reserve(relations.size());
    }

    for (auto &relation: relations)
    {
        auto sender = std::find_if(std::execution::par_unseq, uprm->regions().begin(), uprm->regions().end(), [&relation](auto& region)
        {
           return region->uuid() == relation.sender;
        });

        if(sender == uprm->regions().end())
            continue;

        auto receiver = std::find_if(std::execution::par_unseq, uprm->regions().begin(), uprm->regions().end(), [&relation](auto& region)
        {
            return region->uuid() == relation.recv;
        });

        if(receiver == uprm->regions().end())
            continue;

        if (uplm->add_link(*sender, relation.sender_attrib, *receiver, relation.recv_attrib, ILink::LINK_TYPE::UD))
        {
            d_links_in_ctx[source].push_back(std::make_shared<UnidirectionalLink>(*sender, *receiver, relation.sender_attrib, relation.recv_attrib));

            Q_EMIT (*sender)->LINK_SIGNAL(_UUID_, (*sender)->uuid(), relation.sender_attrib, bp::extract<bp::tuple>((*sender)->effect().attr_link_emit()[relation.sender_attrib]()));
        }
    }
}

void Context::spawn_folder_contents_buttons_as_regions(std::shared_ptr<Region>& parent, uint32_t dir_x, uint32_t dir_y, uint32_t preview_width, uint32_t preview_height)
{
    bp::object value = d_available_plugins["Button"];

    uint32_t btn_width = bp::extract<uint32_t>(value.attr("width"));
    uint32_t btn_height = bp::extract<uint32_t>(value.attr("height"));

    std::vector<glm::vec3> btn_contour{glm::vec3(dir_x + preview_width - btn_width, dir_y + preview_height - btn_height, 1),
                                       glm::vec3(dir_x + preview_width - btn_width, dir_y + preview_height, 1),
                                       glm::vec3(dir_x + preview_width, dir_y + preview_height, 1),
                                       glm::vec3(dir_x + preview_width, dir_y + preview_height - btn_height, 1)};

    bp::dict kwargs;
    kwargs["value"] = false;

    uprm->query_region_insertion(btn_contour, value, parent, kwargs, std::string("__position__"), std::string("__position__"));

    std::vector<glm::vec3> btn_contour2{glm::vec3(dir_x, dir_y + preview_height - btn_height, 1),
                                        glm::vec3(dir_x, dir_y + preview_height, 1),
                                        glm::vec3(dir_x + btn_width, dir_y + preview_height, 1),
                                        glm::vec3(dir_x + btn_width, dir_y + preview_height - btn_height, 1)};
    bp::dict kwargs2;

    uprm->query_region_insertion(btn_contour2, value, parent, kwargs2, std::string("__position__"), std::string("__position__"));
}

void Context::spawn_folder_contents_entry_as_region(const std::vector<glm::vec3>& contour, std::shared_ptr<Region>& parent, const std::string& effect_type, const bp::dict& kwargs)
{
    uprm->query_region_insertion(contour, d_available_plugins[effect_type], parent, kwargs, std::string("__position__"), std::string("__position__"));
}

void Context::spawn_folder_contents_entries_as_regions(std::shared_ptr<Region>& parent, const std::vector<std::string>& children_paths, uint32_t dir_x, uint32_t dir_y, uint32_t dir_width, uint32_t dir_height, uint32_t preview_width, uint32_t preview_height)
{
    uint32_t x_offset = preview_width / 10;
    uint32_t y_offset = preview_height / 6;
    uint32_t y_offset2 = dir_height / 8;

    uint32_t i = 0;
    int32_t y = -1;
    int32_t x = 1;

    for(auto& child_path: children_paths)
    {
        if(i & 1)
            ++x;
        else
        {
            --x;
            ++y;
        }

        std::vector<glm::vec3> contour {glm::vec3(((x_offset + dir_width) * x) + (dir_x + x_offset), ((y_offset2 + dir_height) * y) + (dir_y + y_offset), 1),
                                        glm::vec3(((x_offset + dir_width) * x) + (dir_x + x_offset), ((y_offset2 + dir_height) * y) + (dir_y + y_offset + dir_height), 1),
                                        glm::vec3(((x_offset + dir_width) * x) + (dir_x + x_offset + dir_width), ((y_offset2 + dir_height) * y) + (dir_y + y_offset + dir_height), 1),
                                        glm::vec3(((x_offset + dir_width) * x) + (dir_x + x_offset + dir_width), ((y_offset2 + dir_height) * y) + (dir_y + y_offset), 1)};

        bp::dict kwargs;
        std::string effect_type;

        switch (upfs->entry_type(child_path))
        {
            case SI_TYPE_DIRECTORY:
                kwargs["cwd"] = child_path;
                kwargs["children"] = upfs->cwd_contents_paths(child_path);
                kwargs["is_child"] = true;
                effect_type = "Directory";
                break;
            case SI_TYPE_UNKNOWN_FILE:
            case SI_TYPE_IMAGE_FILE:
            case SI_TYPE_TEXT_FILE:
                kwargs["cwd"] = child_path;
                kwargs["is_child"] = true;
                effect_type = "TextFile";
                break;
        }

        spawn_folder_contents_entry_as_region(contour, parent, effect_type, kwargs);

        ++i;
    }
}

void Context::spawn_folder_contents_as_regions(const std::vector<std::string>& children_paths, const std::string& uuid, const bool with_btns)
{
    auto it = std::find_if(std::execution::par_unseq, uprm->regions().begin(), uprm->regions().end(), [&uuid](auto& region)
    {
        return region->uuid() == uuid;
    });

    if(it != uprm->regions().end())
    {
        auto& region = *it;

        const glm::vec3& tlc = region->aabb()[0];

        const int32_t dir_x = tlc.x + region->transform()[0].z;
        const int32_t dir_y = tlc.y + region->transform()[1].z;
        const uint32_t preview_width = bp::extract<uint32_t>(region->raw_effect().attr("preview_width"));
        const uint32_t preview_height = bp::extract<uint32_t>(region->raw_effect().attr("preview_height"));
        const uint32_t dir_width = bp::extract<uint32_t>(region->raw_effect().attr("icon_width")) * 2;
        const uint32_t dir_height = bp::extract<uint32_t>(region->raw_effect().attr("icon_height")) + bp::extract<uint32_t>(region->raw_effect().attr("text_height"));


        if(with_btns)
            spawn_folder_contents_buttons_as_regions(region, dir_x, dir_y, preview_width, preview_height);

        spawn_folder_contents_entries_as_regions(region, children_paths, dir_x, dir_y, dir_width, dir_height, preview_width, preview_height);
    }
}

void Context::launch_external_application_with_file(const std::string& uuid, const std::string& path)
{
    std::vector<glm::vec3> contour
    {
        glm::vec3(1, 1, 1),
        glm::vec3(1, 1 + s_height * 0.3, 1),
        glm::vec3(1 + s_width * 0.3, 1 + s_height * 0.3, 1),
        glm::vec3(1 + s_width * 0.3, 1, 1)
    };

//    uprm->add_region(contour, d_available_plugins["Container"], 0, kwargs);

    std::shared_ptr<Region> __empty__ = std::make_shared<Region>(contour, d_available_plugins["Container"]);

    upeam->launch_standard_application(uuid, path, __empty__);
}

void Context::terminate_external_application_with_file(const std::string& uuid)
{
    upeam->terminate_application(uuid);
}