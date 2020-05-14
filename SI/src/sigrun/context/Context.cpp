

#include <sigrun/log/Log.hpp>
#include "Context.hpp"
#include <QApplication>
#include <QMainWindow>
#include <sigrun/rendering/IRenderEngine.hpp>
#include <boost/python.hpp>
#include <pysi/PySIEffect.hpp>
#include <sigrun/util/Util.hpp>
#include <sigrun/plugin/Scripting.hpp>

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

        std::for_each(std::execution::par_unseq, plugins.begin(), plugins.end(), [&](const auto& plugin)
        {
            HANDLE_PYTHON_CALL(PY_WARNING, "Plugin does not have the attribute \'region_type\' on module level and is skipped. Try assigning PySIEffect.EffectType.SI_CUSTOM.",
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
                                   case SI_TYPE_PALETTE:
                                   case SI_TYPE_SELECTOR:
                                       break;

                                   default:
                                       d_available_plugins[plugin.first] = *plugin.second;
                                       break;
                               }
            )
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

        HANDLE_PYTHON_CALL(PY_ERROR, "Could not load startup file! A python file called \'StartSIGRun\' is required to be present in plugins folder!",
            bp::import("plugins.StartSIGRun").attr("on_startup")();
        )

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
    return s_height;
}

void Context::set_effect(const std::string& target_uuid, const std::string& effect_name, const std::string& effect_display_name, bp::dict& kwargs)
{
    auto it = std::find_if(std::execution::seq, uprm->regions().begin(), uprm->regions().end(), [&](auto& region)
    {
        return region->uuid() == target_uuid;
    });

    if(it != uprm->regions().end())
    {
        if (it->get()->type() == SI_TYPE_MOUSE_CURSOR)
        {
            d_selected_effects_by_id[target_uuid] = d_available_plugins[effect_name];

            (*std::find_if(std::execution::par_unseq, uprm->regions().begin(), uprm->regions().end(), [&](auto& region)
            {
                return region->type() == SI_TYPE_NOTIFICATION;
            }))->raw_effect().attr("update_message")("Mouse Cursor set to " + effect_display_name);
        }
        else
        {
            it->get()->set_effect(d_available_plugins[effect_name], kwargs);
        }
    }
}

void Context::enable(uint32_t what)
{

}

void Context::disable(uint32_t what)
{

}

void Context::register_new_region(const std::vector<glm::vec3>& contour, const std::string& uuid)
{
    if(contour.size() > 2)
    {
        std::shared_ptr<Region> t(nullptr);
        uprm->query_region_insertion(contour, d_selected_effects_by_id[uuid], t);
    }
}

void Context::register_new_region_via_name(const std::vector<glm::vec3>& contour, const std::string& effect_name, bool as_selector, bp::dict& kwargs)
{
    if(as_selector)
    {
        HANDLE_PYTHON_CALL(PY_WARNING, "The plugin effect for which a selector effect is to be created does not have the attribute \'region_display_name\' on module level.",
            const auto& selector = d_plugins[SI_NAME_EFFECT_SELECTOR];

            Region temp(contour, d_available_plugins[effect_name]);

            kwargs["target_color"] = temp.color();
            kwargs["target_texture"] = temp.raw_effect().attr("texture_path");
            kwargs["target_display_name"] = d_available_plugins[effect_name].attr("region_display_name");
            kwargs["target_name"] = effect_name;

            uprm->add_region(contour, selector, 0, kwargs);
        )
    }
    else
    {
        uprm->add_region(contour, d_available_plugins[effect_name], 0, kwargs);
    }
}

void Context::register_new_region_via_type(const std::vector<glm::vec3>& contour, int id, bp::dict& kwargs)
{
    auto effect = std::find_if(std::execution::par_unseq, d_plugins.begin(), d_plugins.end(), [&id](auto& pair)
    {
       return  pair.second.attr("region_type") == id;
    });

    if(effect != d_plugins.end())
    {
        if(id == SI_TYPE_DIRECTORY)
        {
            std::string k_cwd = std::string(bp::extract<char*>(kwargs["cwd"]));

            if(!k_cwd.empty())
                upfs->set_cwd(k_cwd);

            const std::string& cwd = upfs->cwd();
            const std::vector<std::string> children_paths = upfs->cwd_contents_paths(cwd);

            kwargs["cwd"] = cwd;
            kwargs["children"] = children_paths;
            kwargs["parent"] = "";

            INFO("Region for " + cwd + " created");
        }

        uprm->add_region(contour, effect->second, 0, kwargs);

        if(id == SI_TYPE_MOUSE_CURSOR)
        {
            upim->external_objects()[uprm->regions().back()->uuid()] = std::make_shared<ExternalObject>(ExternalObject::ExternalObjectType::MOUSE);
            uplm->add_link(Context::SIContext()->input_manager()->external_objects()[uprm->regions().back()->uuid()], uprm->regions().back(), SI_CAPABILITY_LINK_POSITION, SI_CAPABILITY_LINK_POSITION);

            INFO("Plugin available for drawing");
        }
    }
}

const std::vector<std::string>& Context::available_plugins_names()
{
    return d_available_plugins_names;
}

void Context::spawn_folder_contents_buttons_as_regions(std::shared_ptr<Region>& parent, uint32_t dir_x, uint32_t dir_y, uint32_t preview_width, uint32_t preview_height)
{
    HANDLE_PYTHON_CALL(PY_WARNING, "Unable to add buttons for browsing folder contents to " + parent->name() + ". Try adding the attributes \'region_width\' and \'region_height\' on module level.",
        bp::object value = d_plugins[SI_NAME_EFFECT_BUTTON];

        uint32_t btn_width = bp::extract<uint32_t>(value.attr("region_width"));
        uint32_t btn_height = bp::extract<uint32_t>(value.attr("region_height"));

        std::vector<glm::vec3> btn_contour{glm::vec3(dir_x + preview_width - btn_width, dir_y + preview_height - btn_height, 1),
                                           glm::vec3(dir_x + preview_width - btn_width, dir_y + preview_height, 1),
                                           glm::vec3(dir_x + preview_width, dir_y + preview_height, 1),
                                           glm::vec3(dir_x + preview_width, dir_y + preview_height - btn_height, 1)};

        bp::dict kwargs;
        kwargs["value"] = false;
        kwargs["parent"] = parent->uuid();

        uprm->query_region_insertion(btn_contour, value, parent, kwargs);

        std::vector<glm::vec3> btn_contour2{glm::vec3(dir_x, dir_y + preview_height - btn_height, 1),
                                            glm::vec3(dir_x, dir_y + preview_height, 1),
                                            glm::vec3(dir_x + btn_width, dir_y + preview_height, 1),
                                            glm::vec3(dir_x + btn_width, dir_y + preview_height - btn_height, 1)};
        bp::dict kwargs2;
        kwargs2["value"] = true;
        kwargs2["parent"] = parent->uuid();

        uprm->query_region_insertion(btn_contour2, value, parent, kwargs2);
    )
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
                effect_name = SI_NAME_EFFECT_DIRECTORY;
                break;
            case SI_TYPE_IMAGE_FILE:
                effect_name = SI_NAME_EFFECT_IMAGEFILE;
                kwargs["cwd"] = child_path;
                break;
            case SI_TYPE_UNKNOWN_FILE:
            case SI_TYPE_TEXT_FILE:
                effect_name = SI_NAME_EFFECT_TEXTFILE;
                kwargs["cwd"] = child_path;
                break;
        }

        kwargs["parent"] = parent->uuid();

        uprm->query_region_insertion(contour, d_plugins[effect_name], parent, kwargs);

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

        HANDLE_PYTHON_CALL(PY_WARNING, "An error occured when spawning the entries of " + region->name() + ". Specify the attributes \'preview_width\', \'preview_height\', \'icon_width\', \'icon_height\' and \'text_height\' on class level in your specific entry classes e.g. TextFile.py",
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
    return d_plugins[name];
}
