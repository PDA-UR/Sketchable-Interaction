

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
    uprcm = std::unique_ptr<CollisionManager>(new CollisionManager);
    upfs = std::unique_ptr<FileSystem>(new FileSystem);
    upeam = std::unique_ptr<ExternalApplicationManager>(new ExternalApplicationManager);
}

void Context::add_startup_regions(const std::unordered_map<std::string, std::unique_ptr<bp::object>>& plugins)
{
    std::for_each(std::execution::par_unseq, plugins.begin(), plugins.end(), [&](const auto& plugin)
    {
        switch (bp::extract<int>(plugin.second->attr("region_type")))
        {
            case SI_TYPE_CANVAS:
            case SI_TYPE_MOUSE_CURSOR:
            case SI_TYPE_NOTIFICATION:
            case SI_TYPE_DIRECTORY:
            case SI_TYPE_BUTTON:
            case SI_TYPE_EXTERNAL_APPLICATION_CONTAINER:
            case SI_TYPE_TEXT_FILE:
            case SI_TYPE_IMAGE_FILE:
            case SI_TYPE_UNKNOWN_FILE:
            case SI_TYPE_CURSOR:
            case SI_TYPE_ENTRY:
                break;

            default:
                d_available_plugins[plugin.first] = *plugin.second;
                break;
        }
    });

    if(!d_available_plugins.empty())
    {
        d_available_plugins_names.reserve(d_available_plugins.size());

        // needs to be this way to function, no idea why. parallelizing attempts are a waste of time
        std::transform(d_available_plugins.begin(), d_available_plugins.end(), std::back_inserter(d_available_plugins_names), [&](const auto& pair)
        {
            return pair.first;
        });
    }

    std::for_each(std::execution::par_unseq, plugins.begin(), plugins.end(), [&](const auto& plugin)
    {
        const std::string& key = plugin.first;
        const std::unique_ptr<bp::object>& value = plugin.second;

        if(!value->is_none())
        {
            HANDLE_PYTHON_CALL(
                switch (bp::extract<int>(value->attr("region_type")))
                {
                    case SI_TYPE_CANVAS:
                    {
                        std::vector<glm::vec3> canvas_contour{glm::vec3(1, 1, 1), glm::vec3(1, s_height - 1, 1),
                                                              glm::vec3(s_width - 1, s_height - 1, 1),
                                                              glm::vec3(s_width - 1, 1, 1)};
                        uprm->add_region(canvas_contour, *value, 0, bp::dict());
                        d_canvas_uuid = uprm->regions().back()->uuid();
                    }
                        break;

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
                        break;

                    case SI_TYPE_BUTTON:
                        break;

                    case SI_TYPE_EXTERNAL_APPLICATION_CONTAINER:
                        break;

                    case SI_TYPE_TEXT_FILE:
                        break;

                    case SI_TYPE_IMAGE_FILE:
                        break;

                    case SI_TYPE_UNKNOWN_FILE:
                        break;

                    case SI_TYPE_CURSOR:
                        break;

                    case SI_TYPE_ENTRY:
                        break;

                    default:
                    {
                        d_available_plugins[key] = *value;
                    }
                        break;
                }
            )
        }
    });
}

void Context::add_canvas_region(const std::unordered_map<std::string, std::unique_ptr<bp::object>>& plugins)
{

}

void Context::add_cursor_regions(const std::unique_ptr<bp::object>& cursor_effect)
{
    HANDLE_PYTHON_CALL(
        uint32_t width_mouse_cursor = bp::extract<uint32_t>(cursor_effect->attr("width"));
        uint32_t height_mouse_cursor = bp::extract<uint32_t>(cursor_effect->attr("height"));

        std::vector<glm::vec3> mouse_contour {glm::vec3(0, 0, 1), glm::vec3(0, height_mouse_cursor, 1), glm::vec3(width_mouse_cursor, height_mouse_cursor, 1), glm::vec3(width_mouse_cursor, 0, 1) };
        uprm->add_region(mouse_contour, *cursor_effect, 0, bp::dict());
        d_mouse_uuid = uprm->regions().back()->uuid();

        upim->external_objects()[d_mouse_uuid] = std::make_shared<ExternalObject>(ExternalObject::ExternalObjectType::MOUSE);
        uplm->add_link(Context::SIContext()->input_manager()->external_objects()[uprm->regions().back()->uuid()], uprm->regions().back(), SI_CAPABILITY_LINK_POSITION, SI_CAPABILITY_LINK_POSITION);

        INFO("Plugin available for drawing");
    )
}

void Context::add_directory_region(const std::unique_ptr<bp::object>& directory_effect)
{
    const std::string& cwd = upfs->cwd();
    const std::vector<std::string> children_paths = upfs->cwd_contents_paths(cwd);

    INFO("Creating Region for " + cwd);

    HANDLE_PYTHON_CALL(
        uint32_t width_directory = bp::extract<uint32_t>(directory_effect->attr("width"));
        uint32_t height_directory = bp::extract<uint32_t>(directory_effect->attr("height"));

        std::vector<glm::vec3> dir_contour {glm::vec3(0, 0, 1), glm::vec3(0, height_directory, 1), glm::vec3(width_directory, height_directory, 1), glm::vec3(width_directory, 0, 1) };

        bp::dict kwargs;

        kwargs["cwd"] = cwd;
        kwargs["children"] = children_paths;
        kwargs["is_child"] = false;

        uprm->add_region(dir_contour, *directory_effect, 0, kwargs);

        INFO("Region for " + cwd + " created");
    )
}

void Context::begin(const std::unordered_map<std::string, std::unique_ptr<bp::object>>& plugins, IRenderEngine* ire, int argc, char** argv)
{
    if(ire)
    {
        d_ire = ire;

        std::for_each(std::execution::par_unseq, plugins.begin(), plugins.end(), [&](auto& plugin)
        {
           d_plugins[plugin.first] = *plugin.second;
        });

        INFO("Creating Qt5 Application...");
        QApplication d_app(argc, argv);
        INFO("Qt5 Application created!");

//        d_app.setOverrideCursor(Qt::BlankCursor);

        d_app.installEventFilter(upim.get());

        d_ire->start(s_width, s_height);

        for(QWidget* pWidget: QApplication::topLevelWidgets())
        {
            d_main_window = qobject_cast<QMainWindow *>(pWidget);

            if (d_main_window)
                break;
        }

        if(!d_main_window)
        {
            ERROR("Main Window could not be created!");
            exit(69);
        }

        add_startup_regions(plugins);

        if(!d_available_plugins.empty())
            d_selected_effects_by_id[d_mouse_uuid] = d_available_plugins[d_available_plugins_names[d_selected_effect_index]];

        d_app.exec();
        INFO("QT5 Application terminated!");
    }
}

void Context::end()
{
    if(d_ire)
        d_ire->stop();
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

QMainWindow* Context::main_window() const
{
    return d_main_window;
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
        HANDLE_PYTHON_CALL(
            (++d_selected_effect_index) %= d_available_plugins_names.size();

            d_selected_effects_by_id[d_mouse_uuid] = d_available_plugins[d_available_plugins_names[d_selected_effect_index]];

            (*std::find_if(std::execution::par_unseq, uprm->regions().begin(), uprm->regions().end(), [&](auto& region)
            {
                return region->type() == SI_TYPE_NOTIFICATION;
            }))->raw_effect().attr("update_message")("Mouse Cursor set to " + d_available_plugins_names[d_selected_effect_index]);
        )
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

void Context::register_new_region_via_name(const std::vector<glm::vec3>& contour, const std::string& name)
{

}

std::vector<std::string> Context::available_plugins_names()
{
    return d_available_plugins_names;
}

void Context::spawn_folder_contents_buttons_as_regions(std::shared_ptr<Region>& parent, uint32_t dir_x, uint32_t dir_y, uint32_t preview_width, uint32_t preview_height)
{
    HANDLE_PYTHON_CALL(
        bp::object value = d_plugins[SI_NAME_EFFECT_BUTTON];

        uint32_t btn_width = bp::extract<uint32_t>(value.attr("width"));
        uint32_t btn_height = bp::extract<uint32_t>(value.attr("height"));

        std::vector<glm::vec3> btn_contour{glm::vec3(dir_x + preview_width - btn_width, dir_y + preview_height - btn_height, 1),
                                           glm::vec3(dir_x + preview_width - btn_width, dir_y + preview_height, 1),
                                           glm::vec3(dir_x + preview_width, dir_y + preview_height, 1),
                                           glm::vec3(dir_x + preview_width, dir_y + preview_height - btn_height, 1)};

        bp::dict kwargs;
        kwargs["value"] = false;

        uprm->query_region_insertion(btn_contour, value, parent, kwargs);

        std::vector<glm::vec3> btn_contour2{glm::vec3(dir_x, dir_y + preview_height - btn_height, 1),
                                            glm::vec3(dir_x, dir_y + preview_height, 1),
                                            glm::vec3(dir_x + btn_width, dir_y + preview_height, 1),
                                            glm::vec3(dir_x + btn_width, dir_y + preview_height - btn_height, 1)};
        bp::dict kwargs2;

        uprm->query_region_insertion(btn_contour2, value, parent, kwargs2);
    )
}

void Context::spawn_folder_contents_entry_as_region(const std::vector<glm::vec3>& contour, std::shared_ptr<Region>& parent, const std::string& effect_name, const bp::dict& kwargs)
{
    uprm->query_region_insertion(contour, d_plugins[effect_name], parent, kwargs);
}

void Context::spawn_folder_contents_entries_as_regions(std::shared_ptr<Region>& parent, const std::vector<std::string>& children_paths, uint32_t dir_x, uint32_t dir_y, uint32_t dir_width, uint32_t dir_height, uint32_t preview_width, uint32_t preview_height)
{
    uint32_t x_offset = preview_width / 10;
    uint32_t y_offset = preview_height / 6;
    uint32_t y_offset2 = dir_height / 8;

    uint32_t i = 0;
    int32_t y = -1;
    int32_t x = 1;

    std::for_each(std::execution::seq, children_paths.begin(), children_paths.end(), [&](auto& child_path)
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
        std::string effect_name;

        switch (upfs->entry_type(child_path))
        {
            case SI_TYPE_DIRECTORY:
                kwargs["cwd"] = child_path;
                kwargs["children"] = upfs->cwd_contents_paths(child_path);
                kwargs["is_child"] = true;
                effect_name = SI_NAME_EFFECT_DIRECTORY;
                break;
            case SI_TYPE_IMAGE_FILE:
                effect_name = SI_NAME_EFFECT_IMAGEFILE;
                kwargs["cwd"] = child_path;
                kwargs["is_child"] = true;
                break;
            case SI_TYPE_UNKNOWN_FILE:
            case SI_TYPE_TEXT_FILE:
                effect_name = SI_NAME_EFFECT_TEXTFILE;
                kwargs["cwd"] = child_path;
                kwargs["is_child"] = true;
                break;
        }

        spawn_folder_contents_entry_as_region(contour, parent, effect_name, kwargs);

        ++i;
    });
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

        HANDLE_PYTHON_CALL(
            const uint32_t preview_width = bp::extract<uint32_t>(region->raw_effect().attr("preview_width"));
            const uint32_t preview_height = bp::extract<uint32_t>(region->raw_effect().attr("preview_height"));
            const uint32_t dir_width = bp::extract<uint32_t>(region->raw_effect().attr("icon_width")) * 2;
            const uint32_t dir_height = bp::extract<uint32_t>(region->raw_effect().attr("icon_height")) + bp::extract<uint32_t>(region->raw_effect().attr("text_height"));

            if(with_btns)
                spawn_folder_contents_buttons_as_regions(region, dir_x, dir_y, preview_width, preview_height);

            spawn_folder_contents_entries_as_regions(region, children_paths, dir_x, dir_y, dir_width, dir_height, preview_width, preview_height);
        )
    }
}

const std::map<std::string, bp::object>& Context::available_plugins() const
{
    return d_available_plugins;
}

const bp::object& Context::plugin_by_name(const std::string& name)
{
    if(MAP_HAS_KEY(d_plugins, name))
        return d_plugins[name];

    return bp::object();
}
