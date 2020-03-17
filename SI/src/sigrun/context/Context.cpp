

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
        for(auto & i : d_links_in_ctx[source])
            uplm->remove_link(i->sender_a(), i->attribute_a(), i->receiver_b(), i->attribute_b(), ILink::LINK_TYPE::UD);

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
            {
                d_links_in_ctx[source].push_back(std::make_shared<UnidirectionalLink>(uprm->regions()[index_sender], uprm->regions()[index_recv], relation.sender_attrib, relation.recv_attrib));
                Q_EMIT uprm->regions()[index_sender]->LINK_SIGNAL(_UUID_, relation.sender_attrib, bp::extract<bp::tuple>(uprm->regions()[index_sender]->effect().attr_link_emit()[relation.sender_attrib]()));
            }
        }
    }
}

void Context::spawn_folder_contents_buttons_as_regions(std::shared_ptr<Region>& parent, int dir_x, int dir_y, int preview_width, int preview_height)
{
    bp::object value = d_available_plugins["Button"];

    int btn_width = bp::extract<int>(value.attr("width"));
    int btn_height = bp::extract<int>(value.attr("height"));

    std::vector<glm::vec3> btn_contour{glm::vec3(dir_x + preview_width - btn_width, dir_y + preview_height - btn_height, 1),
                                       glm::vec3(dir_x + preview_width - btn_width, dir_y + preview_height, 1),
                                       glm::vec3(dir_x + preview_width, dir_y + preview_height, 1),
                                       glm::vec3(dir_x + preview_width, dir_y + preview_height - btn_height, 1)};

    bp::dict kwargs;
    kwargs["value"] = false;

    uprm->query_region_insertion(btn_contour, value, parent, kwargs, std::string("__position__"),std::string("__position__"));

    std::vector<glm::vec3> btn_contour2{glm::vec3(dir_x, dir_y + preview_height - btn_height, 1),
                                        glm::vec3(dir_x, dir_y + preview_height, 1),
                                        glm::vec3(dir_x + btn_width, dir_y + preview_height, 1),
                                        glm::vec3(dir_x + btn_width, dir_y + preview_height - btn_height, 1)};
    bp::dict kwargs2;
    kwargs2["value"] = true;

    uprm->query_region_insertion(btn_contour2, value, parent, kwargs2, std::string("__position__"),std::string("__position__"));
}

void Context::spawn_folder_contents_entry_as_region(const std::vector<glm::vec3>& contour, std::shared_ptr<Region>& parent, const std::string& effect_type, const bp::dict& kwargs)
{
    uprm->query_region_insertion(contour, d_available_plugins[effect_type], parent, kwargs, std::string("__position__"), std::string("__position__"));
}

void Context::spawn_folder_contents_entries_as_regions(std::shared_ptr<Region>& parent, const std::vector<std::string>& children_paths, int dir_x, int dir_y, int dir_width, int dir_height, int preview_width, int preview_height)
{
    int x_offset = preview_width / 10;
    int y_offset = preview_height / 6;
    int y_offset2 = dir_height / 8;

    unsigned i = 0;
    int y = -1;
    int x = 1;

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
    for(auto& r: uprm->regions())
    {
        if (r->uuid() == uuid)
        {
            const int preview_width = bp::extract<int>(r->raw_effect().attr("preview_width"));
            const int preview_height = bp::extract<int>(r->raw_effect().attr("preview_height"));

            const glm::vec3& tlc = r->aabb()[0];

            const int dir_x = tlc.x + r->transform()[0].z;
            const int dir_y = tlc.y + r->transform()[1].z;
            const int dir_width = bp::extract<int>(r->raw_effect().attr("icon_width")) * 2;
            const int dir_height = bp::extract<int>(r->raw_effect().attr("icon_height")) + bp::extract<int>(r->raw_effect().attr("text_height"));

            if(with_btns)
                spawn_folder_contents_buttons_as_regions(r, dir_x, dir_y, preview_width, preview_height);

            spawn_folder_contents_entries_as_regions(r, children_paths, dir_x, dir_y, dir_width, dir_height, preview_width, preview_height);

            break;
        }
    }
}

void Context::embed_winid(int winid)
{
    d_external_winid_to_embedded_app[winid]= QWidget::createWindowContainer(QWindow::fromWinId(winid));

    QMainWindow* pMainWnd;

    for(QWidget* pWidget : QApplication::topLevelWidgets())
    {
        pMainWnd = qobject_cast<QMainWindow*>(pWidget);
        if (pMainWnd)
            break;
    }

    if(pMainWnd)
        d_external_winid_to_embedded_app[winid]->setParent(pMainWnd);

    d_external_winid_to_embedded_app[winid]->show();
}

void Context::destroy_winid(int winid)
{
    if(MAP_HAS_KEY(d_external_winid_to_embedded_app, winid))
    {
        d_external_winid_to_embedded_app[winid]->close();
        delete d_external_winid_to_embedded_app[winid];
        d_external_winid_to_embedded_app.erase(winid);
        INFO("DELETED window: " + std::to_string(winid));
    }
}