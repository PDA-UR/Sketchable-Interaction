

#include <sigrun/log/Log.hpp>
#include "Context.hpp"
#include <QApplication>
#include <QMainWindow>
#include <sigrun/rendering/IRenderEngine.hpp>
#include <memory>
#include <boost/python.hpp>
#include <pysi/PySIEffect.hpp>
#include <sigrun/util/Util.hpp>
#include <sigrun/plugin/Scripting.hpp>
#include <thread>
#include <mutex>
#include <sigrun/SITypes.hpp>
#include <csignal>
#include <cstdlib>
#include <QThread>

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

    uprm = std::make_unique<RegionManager>();
    uplm = std::make_unique<LinkingManager>();
    upim = std::make_unique<InputManager>();
    uprcm = std::make_unique<CollisionManager>();
    upfs = std::make_unique<FileSystem>();
    upeam = std::make_unique<ExternalApplicationManager>();
    upjs = std::make_unique<JobSystem<void, 512>>();
}

void Context::begin(const std::unordered_map<std::string, std::unique_ptr<bp::object>>& plugins, IRenderEngine* ire, int argc, char** argv)
{
    if(ire)
    {
        d_ire = ire;

        for(const auto& [k, v]: plugins)
            d_plugins[k] = *v;

        INFO("Creating Qt5 Application...");
        QApplication d_app(argc, argv);
        INFO("Qt5 Application created!");

        d_app.installEventFilter(upim.get());

        for(const auto& plugin: plugins)
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
        }

        for(const auto& [k, v]: d_available_plugins)
            d_available_plugins_names.push_back(k);

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

        std::thread([&]
        {
            HANDLE_PYTHON_CALL(PY_ERROR, "Could not load startup file! A python file called \'StartSIGRun\' is required to be present in plugins folder!",
                bp::import("plugins.StartSIGRun").attr("on_startup")();
            )
        }).detach();

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

JobSystem<void, 512>* Context::job_system()
{
    return upjs.get();
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
    for(auto it = deo.begin(); it != deo.end();)
    {
        switch(it->second->type())
        {
            case ExternalObject::ExternalObjectType::MOUSE:
            {
                auto& x = upim->mouse_coords().x;
                auto& y = upim->mouse_coords().y;

                auto& px = upim->previous_mouse_coords().x;
                auto& py = upim->previous_mouse_coords().y;

                bp::tuple args = bp::make_tuple(px, py, x, y);

                Q_EMIT it->second->LINK_SIGNAL(_UUID_, "", SI_CAPABILITY_LINK_POSITION, args);
            }
                break;

            case ExternalObject::ExternalObjectType::APPLICATION:
            {
                if(kill(it->second->embedded_object.external_application.pid, 0) < 0
                   || !it->second->embedded_object.external_application.window->isVisible())
                {
                    it->second->embedded_object.external_application.window->close();

                    auto& regions = Context::SIContext()->region_manager()->regions();

                    auto it2 = std::find_if(regions.begin(), regions.end(), [&](auto& region)
                    {
                        return region->uuid() == it->first;
                    });

                    if(it2 != regions.end())
                    {
                        HANDLE_PYTHON_CALL(PY_WARNING, it2->get()->name() + " is intended for deletion, however the signal for deletion could not be invoked!",
                            it2->get()->raw_effect().attr("__signal_deletion__")();
                        )
                    }

                    delete it->second->embedded_object.external_application.window;
                    it->second->embedded_object.external_application.window = nullptr;
                    free(it->second->embedded_object.external_application.file_uuid);
                    it->second->embedded_object.external_application.file_uuid = nullptr;

                    it = deo.erase(it);

                    continue;
                }
                else
                {
                    QWidget* current = it->second->embedded_object.external_application.window;

                    bp::tuple args = bp::make_tuple(current->x() - Context::SIContext()->main_window()->x(), current->y(), current->width(), current->height());
                    current->setProperty("is_resizing", QVariant(false));
                    Q_EMIT it->second->LINK_SIGNAL(_UUID_, "", SI_CAPABILITY_LINK_GEOMETRY, args);
                }
            }
                break;
        }

        ++it;
    }

    int32_t external_application_containers_queue_size = d_external_application_container_insertion_queue.size();

    for(int32_t i = 0; i < external_application_containers_queue_size; ++i)
    {
        const auto& external_app_container_tuple = d_external_application_container_insertion_queue.front();

        QWidget* window = QWidget::createWindowContainer(QWindow::fromWinId(std::get<0>(external_app_container_tuple)));
        window->setGeometry(20, 20, s_width * 0.3 - 40, s_height * 0.3 - 40);
        window->setWindowFlags(Qt::WindowStaysOnTopHint);
        window->setWindowFlags(Qt::ForeignWindow);
        window->setWindowTitle(QString(SI_LINUX_DEFAULT_SI_APP_OPENING.c_str()) + std::get<2>(external_app_container_tuple));

        if(window->x() <= d_main_window->x())
            window->move(d_main_window->x(), 0);

        window->show();

        uint32_t x = window->x() - Context::SIContext()->main_window()->x();
        uint32_t y = window->y();
        uint32_t width = window->width();
        uint32_t height = window->height();

        std::vector<glm::vec3> contour
                {
                        glm::vec3(x, y, 1),
                        glm::vec3(x, y + height, 1),
                        glm::vec3(x + width, y + height, 1),
                        glm::vec3(x + width, y, 1)
                };

        bp::dict kwargs;
        kwargs[SI_LINUX_PID] = std::get<1>(external_app_container_tuple);

        uprm->add_region(contour, d_plugins[SI_NAME_EFFECT_CONTAINER], 0, kwargs);

        auto& container = uprm->regions().back();

        deo[container->uuid()] = std::make_shared<ExternalObject>(ExternalObject::ExternalObjectType::APPLICATION);
        deo[container->uuid()]->embedded_object.external_application.window = window;
        deo[container->uuid()]->embedded_object.external_application.pid = std::get<1>(external_app_container_tuple);
        deo[container->uuid()]->embedded_object.external_application.file_uuid = strdup(std::get<3>(external_app_container_tuple).c_str());

        Context::SIContext()->linking_manager()->add_link(deo[container->uuid()], container, SI_CAPABILITY_LINK_POSITION, SI_CAPABILITY_LINK_POSITION);

        d_external_application_container_insertion_queue.pop();
    }

    int32_t region_queue_size = d_region_insertion_queue.size();

    if(region_queue_size > 50)
    {
        for(int32_t i = 0; i < 50; ++i)
        {
            const auto& region_information_tuple = d_region_insertion_queue.front();

            uprm->add_region(std::get<0>(region_information_tuple), std::get<1>(region_information_tuple), std::get<2>(region_information_tuple), std::get<4>(region_information_tuple));

            if(std::get<3>(region_information_tuple) == SI_TYPE_MOUSE_CURSOR)
            {
                deo[uprm->regions().back()->uuid()] = std::make_shared<ExternalObject>(ExternalObject::ExternalObjectType::MOUSE);
                uplm->add_link(deo[uprm->regions().back()->uuid()], uprm->regions().back(), SI_CAPABILITY_LINK_POSITION, SI_CAPABILITY_LINK_POSITION);

                INFO("Plugin available for drawing");
            }

            d_region_insertion_queue.pop();
        }
    }
    else
    {
        for(int32_t i = 0; i < region_queue_size; ++i)
        {
            const auto& region_information_tuple = d_region_insertion_queue.front();

            uprm->add_region(std::get<0>(region_information_tuple), std::get<1>(region_information_tuple), std::get<2>(region_information_tuple), std::get<4>(region_information_tuple));

            if(std::get<3>(region_information_tuple) == SI_TYPE_MOUSE_CURSOR)
            {
                deo[uprm->regions().back()->uuid()] = std::make_shared<ExternalObject>(ExternalObject::ExternalObjectType::MOUSE);
                uplm->add_link(deo[uprm->regions().back()->uuid()], uprm->regions().back(), SI_CAPABILITY_LINK_POSITION, SI_CAPABILITY_LINK_POSITION);

                INFO("Plugin available for drawing");
            }

            d_region_insertion_queue.pop();
        }
    }

    int32_t link_queue_size = d_link_emission_queue.size();

    for(int32_t i = 0; i < link_queue_size; ++i)
    {
        const auto& link_tuple = d_link_emission_queue.front();
        auto it = std::find_if(uprm->regions().begin(), uprm->regions().end(), [&](auto& region)
        {
            return region->uuid() == std::get<1>(link_tuple);
        });

        if(it != uprm->regions().end())
        {
            (*it)->register_link_event({std::get<0>(link_tuple), std::get<2>(link_tuple)});
            Q_EMIT (*it)->LINK_SIGNAL(std::get<0>(link_tuple), (*it)->uuid(), std::get<2>(link_tuple), std::get<3>(link_tuple));
        }

        d_link_emission_queue.pop();
    }

    std::thread([&]
    {
        upim->update();
        uprcm->collide(uprm->regions());
        uprm->update();
    }).join();
}

uint32_t Context::width()
{
    return s_width;
}

uint32_t Context::height()
{
    return s_height;
}

void Context::set_effect(const std::string& target_uuid, const std::string& effect_name, const std::string& effect_display_name, bp::dict& kwargs)
{
    auto it = std::find_if(uprm->regions().begin(), uprm->regions().end(), [&](auto& region)
    {
        return region->uuid() == target_uuid;
    });

    if(it != uprm->regions().end())
    {
        if (it->get()->type() == SI_TYPE_MOUSE_CURSOR)
        {
            d_selected_effects_by_id[target_uuid] = d_available_plugins[effect_name];

            (*std::find_if(uprm->regions().begin(), uprm->regions().end(), [&](auto& region)
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
        d_region_insertion_queue.emplace(contour, d_selected_effects_by_id[uuid], 0, -1, bp::dict());
    }
}

void Context::register_new_region_via_name(const std::vector<glm::vec3>& contour, const std::string& effect_name, bool as_selector, bp::dict& kwargs)
{
    if(as_selector)
    {
        HANDLE_PYTHON_CALL(PY_WARNING, "The plugin effect for which a selector effect is to be created does not have the attribute \'region_display_name\' on module level.",
            Region temp(contour, d_available_plugins[effect_name]);

            kwargs["target_color"] = temp.color();
            kwargs["target_texture"] = temp.raw_effect().attr("texture_path");
            kwargs["target_display_name"] = d_available_plugins[effect_name].attr("region_display_name");
            kwargs["target_name"] = effect_name;

            d_region_insertion_queue.emplace(contour, d_plugins[SI_NAME_EFFECT_SELECTOR], 0, -1, kwargs);
        )
    }
    else
    {
        d_region_insertion_queue.emplace(contour, d_available_plugins[effect_name], 0, -1, kwargs);
    }
}

void Context::register_new_region_via_type(const std::vector<glm::vec3>& contour, int id, bp::dict& kwargs)
{
    HANDLE_PYTHON_CALL(PY_ERROR, "Error. Could not add region!.",
        auto effect = std::find_if(d_plugins.begin(), d_plugins.end(), [&id](auto& pair)
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
                const std::vector<int> children_types = upfs->cwd_contents_types(children_paths);

                bp::list children;

                for(int32_t i = 0; i < children_paths.size(); ++i)
                    children.append(bp::make_tuple(children_paths[i], children_types[i]));

                kwargs["cwd"] = cwd;
                kwargs["children"] = children;
            }

            d_region_insertion_queue.emplace(contour, effect->second, 0, id, kwargs);
        }
    )
}

void Context::register_link_event_emission(const std::string& event_uuid, const std::string& sender_uuid, const std::string& sender_attribute, const bp::object& args)
{
    d_link_emission_queue.emplace(event_uuid, sender_uuid, sender_attribute, args);
}


void Context::register_new_application_container(uint64_t winid, uint64_t pid, const QString &window_name, const std::string &file_region_uuid)
{
   d_external_application_container_insertion_queue.emplace(winid, pid, window_name, file_region_uuid);
}

void Context::unregister_external_application(const std::string& file_uuid)
{
    auto it = std::find_if(deo.begin(), deo.end(), [&file_uuid](const auto& pair) -> bool
    {
        return std::get<1>(pair)->type() == ExternalObject::APPLICATION && std::get<1>(pair)->embedded_object.external_application.file_uuid == file_uuid;
    });

    if(it != deo.end())
    {
        auto it2 = std::find_if(Context::SIContext()->region_manager()->regions().begin(), Context::SIContext()->region_manager()->regions().end(), [&it](auto& region)
        {
            return region->uuid() == it->first;
        });

        if(it2 != Context::SIContext()->region_manager()->regions().end())
        {
            Context::SIContext()->linking_manager()->remove_link(it->second, *it2, SI_CAPABILITY_LINK_POSITION, SI_CAPABILITY_LINK_POSITION);
            kill(deo[it->first]->embedded_object.external_application.pid, SIGTERM);
        }
    }
}

const std::vector<std::string>& Context::available_plugins_names()
{
    return d_available_plugins_names;
}

const std::map<std::string, bp::object>& Context::available_plugins() const
{
    return d_available_plugins;
}

const bp::object& Context::plugin_by_name(const std::string& name)
{
    return d_plugins[name];
}

std::unordered_map<std::string, std::shared_ptr<ExternalObject>> &Context::external_objects()
{
    return deo;
}
