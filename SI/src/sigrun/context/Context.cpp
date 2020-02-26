

#include <sigrun/log/Log.hpp>
#include "Context.hpp"
#include <QApplication>
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
{SIGRUN
    self = this;

    uprm = std::unique_ptr<RegionManager>(new RegionManager);
    uplm = std::unique_ptr<LinkingManager>(new LinkingManager);
    upim = std::unique_ptr<InputManager>(new InputManager);
    upcm = std::unique_ptr<Capability>(new Capability);
    uprcm = std::unique_ptr<CollisionManager>(new CollisionManager);
    upfs = std::unique_ptr<FileSystem>(new FileSystem);
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

                        int x = s_width / 2 - width / 2;

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
                if (bp::extract<int>(value->attr("region_type")) == SI_TYPE_CANVAS)
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
    int width_mouse_cursor = bp::extract<int>(cursor_effect->attr("width"));
    int height_mouse_cursor = bp::extract<int>(cursor_effect->attr("height"));

    std::vector<glm::vec3> mouse_contour {glm::vec3(0, 0, 1), glm::vec3(0, height_mouse_cursor, 1), glm::vec3(width_mouse_cursor, height_mouse_cursor, 1), glm::vec3(width_mouse_cursor, 0, 1) };
    uprm->add_region(mouse_contour, *cursor_effect, 0, bp::dict());
    uplm->add_link_to_object(uprm->regions().back(), ExternalObject::ExternalObjectType::MOUSE);
    d_mouse_uuid = uprm->regions().back()->uuid();

    INFO("Plugin available for drawing");
}

void Context::add_directory_region(const std::unique_ptr<bp::object>& directory_effect)
{
    const std::string& cwd = upfs->cwd();
    const std::vector<std::string> children_paths = upfs->cwd_contents_paths(cwd);

    INFO("Creating Region for " + cwd);

    int width_directory = bp::extract<int>(directory_effect->attr("width"));
    int height_directory = bp::extract<int>(directory_effect->attr("height"));

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

        for(const auto& item: plugins)
            upcm->add_capabilities(*item.second);

        d_app.setOverrideCursor(Qt::BlankCursor);


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

int Context::width()
{
    return s_width;
}

int Context::height()
{
    return s_height;
}

void Context::enable(int what)
{

}

void Context::disable(int what)
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

    std::vector<int> indices;
    indices.reserve(6);

    int i = 0;
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
        for (int i = 0; i < d_links_in_ctx[source].size(); ++i)
            uplm->remove_link(d_links_in_ctx[source][i]->sender_a(), d_links_in_ctx[source][i]->attribute_a(), d_links_in_ctx[source][i]->receiver_b(), d_links_in_ctx[source][i]->attribute_b(), ILink::LINK_TYPE::UD);

        d_links_in_ctx.erase(source);
    }
}

void Context::remove_linking_relations(const std::vector<LinkRelation> &relations, const std::string &source)
{
    if (MAP_HAS_KEY(d_links_in_ctx, source))
    {
        auto it = d_links_in_ctx[source].begin();

        while (it != d_links_in_ctx[source].end())
        {
            if (std::find_if(relations.begin(), relations.end(),
                             [&it](const LinkRelation &x)
                             {
                                 return it->get()->sender_a()->uuid() == x.sender &&
                                        it->get()->attribute_a() == x.sender_attrib &&
                                        it->get()->receiver_b()->uuid() == x.recv &&
                                        it->get()->attribute_b() == x.recv_attrib;
                             }) == relations.end())
            {
                uplm->remove_link(it->get()->sender_a(), it->get()->attribute_a(), it->get()->receiver_b(), it->get()->attribute_b(), ILink::LINK_TYPE::UD);
                it = d_links_in_ctx[source].erase(it);
            }
            else
                ++it;
        }
    }
}

void Context::create_linking_relations(const std::vector<LinkRelation> &relations, const std::string &source)
{
    if (!MAP_HAS_KEY(d_links_in_ctx, source))
    {
        d_links_in_ctx[source] = std::vector<std::shared_ptr<ILink>>();

        for (auto &relation: relations)
        {
            int index_sender = -1;
            int index_recv = -1;

            for (int i = 0; i < uprm->regions().size(); ++i)
            {
                auto &region = uprm->regions()[i];

                if (region->uuid() == relation.sender)
                    index_sender = i;

                if (region->uuid() == relation.recv)
                    index_recv = i;
            }

            if (index_sender != -1 && index_recv != -1)
            {
                if (uplm->add_link(uprm->regions()[index_sender], relation.sender_attrib, uprm->regions()[index_recv], relation.recv_attrib, ILink::LINK_TYPE::UD))
                    d_links_in_ctx[source].push_back(std::make_shared<UnidirectionalLink>(uprm->regions()[index_sender], uprm->regions()[index_recv], relation.sender_attrib, relation.recv_attrib));
            }
        }
    }
}

void Context::spawn_folder_contents_as_regions(const std::vector<std::string>& children_paths, const std::string& uuid)
{
    for(auto& r: uprm->regions())
    {
        if (r->uuid() == uuid)
        {
            glm::vec3 tlc = r->aabb()[0] * r->transform();
            glm::vec3 brc = r->aabb()[2] * r->transform();
            tlc /= tlc.z;
            brc /= brc.z;

            int num_items = children_paths.size();

            const int& dir_x = tlc.x;
            const int& dir_y = tlc.y;
            const int& dir_width = r->width();
            const int& dir_height = r->height();

            for(auto& [key, value]: d_available_plugins)
            {
                if (!value.is_none())
                {
                    switch (bp::extract<int>(value.attr("region_type")))
                    {
                        case SI_TYPE_BUTTON:
                        {
                            int btn_width = bp::extract<int>(value.attr("width"));
                            int btn_height = bp::extract<int>(value.attr("height"));

                            std::vector<glm::vec3> btn_contour{glm::vec3(brc.x - btn_width, brc.y - btn_height, 1),
                                                               glm::vec3(brc.x - btn_width, brc.y, 1),
                                                               glm::vec3(brc.x, brc.y, 1),
                                                               glm::vec3(brc.x, brc.y - btn_height, 1)};
                            bp::dict kwargs;
                            kwargs["value"] = false;

                            uprm->query_region_insertion(btn_contour, value, r, kwargs, std::string("__position__"),
                                                         std::string(
                                                                 "__position__")); // cannot link what does not exist though

                            std::vector<glm::vec3> btn_contour2{glm::vec3(0, brc.y - btn_height, 1),
                                                                glm::vec3(0, brc.y, 1),
                                                                glm::vec3(btn_width, brc.y, 1),
                                                                glm::vec3(btn_width, brc.y - btn_height, 1)};
                            bp::dict kwargs2;
                            kwargs2["value"] = true;

                            uprm->query_region_insertion(btn_contour2, value, r, kwargs2, std::string("__position__"),std::string("__position__"));
                        }
                    }
                }
            }

            int width_directory = 130;
            int height_directory = 125;

            int x_offset = 25;
            int y_offset = 125;

            int i = 0;
            int y = -1;
            int x = 1;

            for(auto& child_path: children_paths)
            {
                if(!(i & 1))
                {
                    --x;
                    ++y;
                }
                else
                    ++x;

                std::vector<glm::vec3> contour {glm::vec3(dir_x + x_offset + x * width_directory, dir_y + y_offset + y * height_directory, 1),
                                                glm::vec3(dir_x + x_offset + x * width_directory, dir_y + y_offset + y * height_directory + height_directory + 35, 1),
                                                glm::vec3(dir_x + dir_width + x * width_directory + width_directory, dir_y + y_offset + y * height_directory + height_directory + 35, 1),
                                                glm::vec3(dir_x + dir_width + x * width_directory + width_directory, dir_y + y_offset + y * height_directory, 1) };

                switch (upfs->entry_type(child_path))
                {
                    case SI_TYPE_DIRECTORY:
                    {
                        bp::object value = d_available_plugins["Directory"];

                        bp::dict kwargs;
                        kwargs["cwd"] = child_path;
                        kwargs["children"] = upfs->cwd_contents_paths(child_path);
                        kwargs["is_child"] = true;

                        uprm->query_region_insertion(contour, value, r, kwargs, std::string("__position__"), std::string("__position__"));
                    }
                    break;
                    case SI_TYPE_IMAGE_FILE:
                    case SI_TYPE_TEXT_FILE:
                    case SI_TYPE_UNKNOWN_FILE:
                    {
                        bp::object value = d_available_plugins["TextFile"];

                        bp::dict kwargs;
                        kwargs["cwd"] = child_path;
                        kwargs["is_child"] = true;
                        uprm->query_region_insertion(contour, value, r, kwargs, std::string("__position__"), std::string("__position__"));
                    }
                    break;
                }

                ++i;
            }

            break;
        }
    }
}