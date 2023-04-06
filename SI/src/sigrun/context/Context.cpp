
#include <sigrun/log/Log.hpp>
#include "Context.hpp"
#include <QApplication>
#include <QMainWindow>
#include <sigrun/rendering/IRenderEngine.hpp>
#include <memory>
#include <boost/python.hpp>
#include <pysi/PySIEffect.hpp>
#include <sigrun/util/Util.hpp>
#include <sigrun/SITypes.hpp>
#include <csignal>
#include <cstdlib>
#include <QThread>
#include <QScreen>
#include <sigrun/plugin/PythonGlobalInterpreterLockGuard.hpp>
#include <sigrun/log/CrashDump.hpp>
#include <sigrun/rendering/qml/items/PlotItem.hpp>
#include <sigrun/rendering/qml/items/VideoItem.hpp>
#include <sigrun/util/Benchmark.hpp>
#include <chrono>
#include <QTest>

#define NEW_REGIONS_PER_FRAME 50

namespace bp = boost::python;

Context* Context::self = nullptr;

Context::~Context()
{
    INFO("Destroying Context...");
    upjs.release();
    p_py_garbage_collector = nullptr;


    for(auto& r: uprm->regions()) {
        r->raw_effect().attr("__on_destroy__")();
    }

    INFO("Destroyed Context");
}

Context::Context()
{
    self = this;

    uprm = std::make_unique<RegionManager>();
    uplm = std::make_unique<LinkingManager>();
    upim = std::make_unique<InputManager>();
    uprcm = std::make_unique<CollisionManager>();
    upeam = std::make_unique<ExternalApplicationManager>();
    upjs = std::make_unique<JobSystem<void, 512>>();
    uptm = std::make_unique<TangibleManager>();
    logfile.open("menu_latency.csv", std::ios_base::app);
    logfile << "time\n";


//    p_py_garbage_collector = new bp::object(bp::import(SI_PYTHON_GARBAGE_COLLECTOR));
}

void Context::begin(const std::unordered_map<std::string, std::unique_ptr<bp::object>>& plugins, IRenderEngine* ire, IPhysicalEnvironment* ros, int argc, char** argv)
{
    if (!ire)
        return;

    INFO("Creating Qt5 Application...");
    QApplication d_app(argc, argv);
    s_width = QApplication::primaryScreen()->geometry().width();
    s_height = QApplication::primaryScreen()->geometry().height();
    INFO("Qt5 Application created!");

    upshg = std::make_unique<SpatialHashGrid>(s_width, s_height, 20, 20);

    d_app.installEventFilter(upim.get());

    qmlRegisterType<PlotItem>("siqml", 1, 0, "PlotItem");
    qmlRegisterType<VideoItem>("siqml", 1, 0, "VideoItem");

    d_ire = ire;
    d_ros = ros;

    std::string tmp;

    for(const auto& [k, v]: plugins)
    {
        d_plugins[k] = *v;

        HANDLE_PYTHON_CALL(PY_WARNING, "Plugin does not have the attribute \'regiontype\' as static class member and is skipped. Try assigning PySIEffect.EffectType.SI_CUSTOM.",
            switch (bp::extract<int>(v->attr(v->attr(SI_INTERNAL_NAME)).attr(SI_INTERNAL_REGION_TYPE)))
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
                case SI_TYPE_CUSTOM_NON_DRAWABLE:
                    break;

                default:
                {
                    d_available_plugins[k] = d_plugins[k];
                    d_available_plugins_names.push_back(k);
                    tmp += k + ", ";
                }
                break;
            }
        )
    }

    INFO("Drawable Plugins: " + tmp.substr(0, tmp.length() - 2));

    HANDLE_PYTHON_CALL(PY_ERROR, "Could not load startup file! A python file called \'StartSIGRun\' is required to be present in plugins folder!",
        bp::import(SI_START_FILE).attr(SI_START_FUNCTION)();
    )

    // sequence matters
    if(d_ros)
        d_ros->start(argc, argv);

    d_ire->start(s_width, s_height, 120);

    d_app.exec();
    INFO("QT5 Application terminated!");
}

void Context::end()
{
    if(d_ros)
        d_ros->stop();

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

TangibleManager* Context::tangible_manager()
{
    return uptm.get();
}

SpatialHashGrid* Context::spatial_hash_grid()
{
    return upshg.get();
}

Context* Context::SIContext()
{
    return self;
}

QGraphicsView* Context::main_window() const
{
    return d_main_window;
}

void Context::set_main_window()
{
    for(QWidget* pWidget: QApplication::topLevelWidgets())
    {
        d_main_window = qobject_cast<QGraphicsView *>(pWidget);

        if (d_main_window)
            break;
    }

    if(!d_main_window)
    {
        ERROR("Main Window could not be created!");
        exit(6);
    }
}

void Context::update()
{
    perform_input_update();
    uprm->update();

//    d_ros->update();
    perform_region_insertion();
    perform_link_events();
    perform_collision_update();
//    perform_link_events();

    perform_external_object_update();
    perform_external_application_registration();
}

uint32_t Context::width()
{
    return s_width;
}

uint32_t Context::height()
{
    return s_height;
}

void Context::set_effect(const std::string& target_uuid, const std::string& effect_name, bp::dict& kwargs)
{
    for(auto& region: uprm->regions())
    {
        if(region->uuid() == target_uuid)
        {
            if (region->type() == SI_TYPE_MOUSE_CURSOR || region->name() == "__ Pen __" || region->name() == "__ Tip __")
            {
                d_selected_effects_by_id[target_uuid] = d_available_plugins[effect_name];
            }
            else
            {
                region->set_effect(d_available_plugins[effect_name], kwargs);
            }
        }
    }

//    auto it = std::find_if(uprm->regions().begin(), uprm->regions().end(), [&](auto &region)
//    {
//        return region->uuid() == target_uuid;
//    });
//
//    if (!(it != uprm->regions().end()))
//        return;
//
//    if (it->get()->type() == SI_TYPE_MOUSE_CURSOR || it->get()->name() == "__ Pen __")
//        d_selected_effects_by_id[target_uuid] = d_available_plugins[effect_name];
//    else
//        it->get()->set_effect(d_available_plugins[effect_name], kwargs);
}

void Context::enable(uint32_t what)
{
    if(what & SI_CRASH_DUMP_FLAG)
    {
        signal(SIGSEGV, CrashDump::dump_crash_information);
        signal(SIGABRT, CrashDump::dump_crash_information);
    }

    if(what & SI_ANTI_ALIASING_OFF)
    {
        d_ire->disable_anti_aliasing();
    }

    if(what & SI_ANTI_ALIASING_4x)
    {
        d_ire->enable_anti_aliasing(4);

    }

    if(what & SI_ANTI_ALIASING_8x)
    {
        d_ire->enable_anti_aliasing(8);
    }

    if(what & SI_ANTI_ALIASING_16x)
    {
        d_ire->enable_anti_aliasing(16);
    }
}

void Context::disable(uint32_t what)
{
    if(what & SI_CRASH_DUMP_FLAG)
    {
        signal(SIGSEGV, nullptr);
        signal(SIGABRT, nullptr);
    }

    if(what & SI_ANTI_ALIASING_OFF || what & SI_ANTI_ALIASING_4x || what & SI_ANTI_ALIASING_8x || what & SI_ANTI_ALIASING_16x)
        d_ire->disable_anti_aliasing();
}

void Context::register_new_region_from_object(const bp::object &object, const bp::dict &qml)
{
    uprm->add_region(object, qml);
}

void Context::register_new_region(const std::vector<glm::vec3>& contour, const std::string& uuid, const bp::dict& kwargs)
{
    if(contour.size() > 5 && !kwargs.has_key("resampling"))
        d_region_insertion_queue.emplace(contour, d_selected_effects_by_id[uuid], -1, kwargs);
    else
    {
        d_region_insertion_queue.emplace(contour, d_selected_effects_by_id[uuid], -1, kwargs);
    }
}

void Context::register_new_region_via_name(const std::vector<glm::vec3>& contour, const std::string& effect_name, bool as_selector, bp::dict& kwargs)
{
    int id = bp::extract<int>(d_plugins[effect_name].attr(d_plugins[effect_name].attr(SI_INTERNAL_NAME)).attr(SI_INTERNAL_REGION_TYPE));

    if(!as_selector)
    {
        d_region_insertion_queue.emplace(contour, d_plugins[effect_name], id, kwargs);
        return;
    }

    HANDLE_PYTHON_CALL(PY_WARNING, "The plugin effect for which a selector effect is to be created does not have the attribute \'region_display_name\' as a static class member.",
        kwargs["is_selector"] = true;
        Region temp(std::vector<glm::vec3>{{0, 0, 1}, {0, 1, 1}, {1, 1, 1}, {1, 0, 1}}, d_plugins[effect_name], 0 , 0, kwargs);
        kwargs[SI_SELECTOR_TARGET_COLOR] = temp.color();
        kwargs[SI_SELECTOR_TARGET_TEXTURE] = temp.raw_effect().attr("texture_path");
        kwargs[SI_SELECTOR_TARGET_DISPLAY_NAME] = d_plugins[effect_name].attr(d_plugins[effect_name].attr(SI_INTERNAL_NAME)).attr(SI_INTERNAL_REGION_DISPLAY_NAME);
        kwargs[SI_SELECTOR_TARGET_NAME] = effect_name;

        d_region_insertion_queue.emplace(contour, d_plugins[SI_NAME_EFFECT_SELECTOR], id, kwargs);
    )
}

void Context::register_new_region_via_type(const std::vector<glm::vec3>& contour, int id, bp::dict& kwargs)
{
    for(auto& pair: d_plugins)
    {
        if(pair.second.attr(pair.second.attr(SI_INTERNAL_NAME)).attr(SI_INTERNAL_REGION_TYPE) == id)
        {
            d_region_insertion_queue.emplace(contour, pair.second, id, kwargs);
            break;
        }
    }

//    HANDLE_PYTHON_CALL(PY_ERROR, "Error. Could not add region!.",
//        auto effect = std::find_if(d_plugins.begin(), d_plugins.end(), [&id](auto& pair)
//        {
//           return  pair.second.attr(pair.second.attr(SI_INTERNAL_NAME)).attr(SI_INTERNAL_REGION_TYPE) == id;
//        });
//
//        if(effect != d_plugins.end())
//            d_region_insertion_queue.emplace(contour, effect->second, id, kwargs);
//    )
}

void Context::register_region_via_class_object(const std::vector<glm::vec3>& contour, bp::object& clazz, bp::dict& kwargs)
{
    for(auto& pair: d_plugins)
    {
        if(pair.second.attr(SI_INTERNAL_NAME) == clazz.attr(SI_INTERNAL_NAME))
        {
            int id = bp::extract<int>(clazz.attr(clazz.attr(SI_INTERNAL_NAME)).attr(SI_INTERNAL_REGION_TYPE));
            d_region_insertion_queue.emplace(contour, pair.second, id, kwargs);
            break;
        }
    }

//    auto effect = std::find_if(d_plugins.begin(), d_plugins.end(), [&clazz](auto& pair)
//    {
//        return pair.second.attr(SI_INTERNAL_NAME) == clazz.attr(SI_INTERNAL_NAME);
//    });
//
//    if(effect != d_plugins.end())
//    {
//        int id = bp::extract<int>(clazz.attr(clazz.attr(SI_INTERNAL_NAME)).attr(SI_INTERNAL_REGION_TYPE));
//        d_region_insertion_queue.emplace(contour, effect->second, id, kwargs);
//    }
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
    std::vector<std::string> ret;
    return d_available_plugins_names;
}

const std::unordered_map<std::string, bp::object>& Context::available_plugins() const
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

void Context::perform_external_object_update()
{
    for(auto it = deo.begin(); it != deo.end();)
    {
        switch(it->second->type())
        {
            case ExternalObject::ExternalObjectType::MOUSE:
                perform_mouse_update(it);
                break;

            case ExternalObject::ExternalObjectType::APPLICATION:
                perform_external_application_update(it);
                break;
        }

        ++it;
    }
}

void Context::perform_mouse_update(std::unordered_map<std::string, std::shared_ptr<ExternalObject>>::iterator& it)
{
    auto& x = upim->mouse_coords().x;
    auto& y = upim->mouse_coords().y;

    auto& px = upim->previous_mouse_coords().x;
    auto& py = upim->previous_mouse_coords().y;

    bp::tuple args = bp::make_tuple(x - px, y - py, x, y);
    //bp::tuple args = bp::make_tuple(px, py, x, y);
    Q_EMIT it->second->LINK_SIGNAL(_UUID_, "", SI_CAPABILITY_LINK_POSITION, args);
}

void Context::perform_external_application_update(std::unordered_map<std::string, std::shared_ptr<ExternalObject>>::iterator& it)
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
            it2->get()->effect()->d_flagged_for_deletion = true;

        delete it->second->embedded_object.external_application.window;
        it->second->embedded_object.external_application.window = nullptr;
        free(it->second->embedded_object.external_application.file_uuid);
        it->second->embedded_object.external_application.file_uuid = nullptr;

        it = deo.erase(it);

        return;
    }

    QWidget* current = it->second->embedded_object.external_application.window;
    Q_EMIT it->second->LINK_SIGNAL(_UUID_, "", SI_CAPABILITY_LINK_GEOMETRY, bp::make_tuple(current->x() - Context::SIContext()->main_window()->x(), current->y(), current->width(), current->height()));
}

void Context::perform_external_application_registration()
{
    int32_t external_application_containers_queue_size = d_external_application_container_insertion_queue.size();

    for(int32_t i = 0; i < external_application_containers_queue_size; ++i)
    {
        const auto& external_app_container_tuple = d_external_application_container_insertion_queue.front();

        QWidget* window = QWidget::createWindowContainer(QWindow::fromWinId(std::get<0>(external_app_container_tuple)));
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

        uprm->add_region(contour, d_plugins[SI_NAME_EFFECT_CONTAINER], kwargs);

        auto& container = uprm->regions().back();

        deo[container->uuid()] = std::make_shared<ExternalObject>(ExternalObject::ExternalObjectType::APPLICATION);
        deo[container->uuid()]->embedded_object.external_application.window = window;
        deo[container->uuid()]->embedded_object.external_application.pid = std::get<1>(external_app_container_tuple);
        deo[container->uuid()]->embedded_object.external_application.file_uuid = strdup(std::get<3>(external_app_container_tuple).c_str());

        Context::SIContext()->linking_manager()->add_link(deo[container->uuid()], container, SI_CAPABILITY_LINK_POSITION, SI_CAPABILITY_LINK_POSITION);

        d_external_application_container_insertion_queue.pop();
    }
}

void Context::perform_region_insertion()
{
//    while(!d_region_insertion_queue.empty())
//    {
//        const auto& region_information_tuple = d_region_insertion_queue.front();
//
//        uprm->add_region(std::get<0>(region_information_tuple), std::get<1>(region_information_tuple), std::get<3>(region_information_tuple));
//
//        if(std::get<2>(region_information_tuple) == SI_TYPE_MOUSE_CURSOR)
//        {
//            deo[uprm->regions().back()->uuid()] = std::make_shared<ExternalObject>(ExternalObject::ExternalObjectType::MOUSE);
//            uplm->add_link(deo[uprm->regions().back()->uuid()], uprm->regions().back(), SI_CAPABILITY_LINK_POSITION, SI_CAPABILITY_LINK_POSITION);
//
//            INFO("Plugin available for drawing");
//        }
//
//        d_region_insertion_queue.pop();
//    }
//
//    QApplication::processEvents(QEventLoop::ExcludeSocketNotifiers);

    int32_t region_queue_size = d_region_insertion_queue.size();

    if(region_queue_size > 0)
    { // insert file writing here
//        SI_BENCHMARK_SCOPE(
                for(int32_t i = 0; i < ((region_queue_size > NEW_REGIONS_PER_FRAME) ? NEW_REGIONS_PER_FRAME: region_queue_size); ++i)
                {
                    const auto& region_information_tuple = d_region_insertion_queue.front();
                    uprm->add_region(std::get<0>(region_information_tuple), std::get<1>(region_information_tuple), std::get<3>(region_information_tuple));

                    if(std::get<2>(region_information_tuple) == SI_TYPE_MOUSE_CURSOR)
                    {
                        deo[uprm->regions().back()->uuid()] = std::make_shared<ExternalObject>(ExternalObject::ExternalObjectType::MOUSE);
                        uplm->add_link(deo[uprm->regions().back()->uuid()], uprm->regions().back(), SI_CAPABILITY_LINK_POSITION, SI_CAPABILITY_LINK_POSITION);

                        INFO("Plugin available for drawing");
                    }

                    d_region_insertion_queue.pop();
                }

                QApplication::processEvents(QEventLoop::ExcludeSocketNotifiers);
//        )
        long ms = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count();
        logfile << "end:" << ms << '\n';
    }
    else
    {
        logfile << "-\n";
        for(int32_t i = 0; i < ((region_queue_size > NEW_REGIONS_PER_FRAME) ? NEW_REGIONS_PER_FRAME: region_queue_size); ++i)
        {
            const auto& region_information_tuple = d_region_insertion_queue.front();
            uprm->add_region(std::get<0>(region_information_tuple), std::get<1>(region_information_tuple), std::get<3>(region_information_tuple));

            if(std::get<2>(region_information_tuple) == SI_TYPE_MOUSE_CURSOR)
            {
                deo[uprm->regions().back()->uuid()] = std::make_shared<ExternalObject>(ExternalObject::ExternalObjectType::MOUSE);
                uplm->add_link(deo[uprm->regions().back()->uuid()], uprm->regions().back(), SI_CAPABILITY_LINK_POSITION, SI_CAPABILITY_LINK_POSITION);

                INFO("Plugin available for drawing");
            }

            d_region_insertion_queue.pop();
        }

        QApplication::processEvents(QEventLoop::ExcludeSocketNotifiers);
    }

    if(d_region_insertion_queue.empty() && region_queue_size > 0)
    {
//        SI_BENCHMARK_STOP;
//        exit(0);
    }
}

void Context::perform_link_events()
{
    int32_t link_queue_size = d_link_emission_queue.size();

    for(int32_t i = 0; i < link_queue_size; ++i)
    {
        const auto& link_tuple = d_link_emission_queue.front();
        bool found = false;

        for(auto& region: uprm->regions())
        {
            if(region->uuid() == std::get<1>(link_tuple))
            {
                region->register_link_event({std::get<0>(link_tuple), std::get<2>(link_tuple)});
                Q_EMIT region->LINK_SIGNAL(std::get<0>(link_tuple), region->uuid(), std::get<2>(link_tuple), std::get<3>(link_tuple));
                break;
            }
        }

        d_link_emission_queue.pop();
    }
}

void Context::perform_input_update()
{
    upim->update();
}

void Context::perform_collision_update()
{
    uprcm->collide(uprm->regions());
}

void Context::exclude_plugins(const std::vector<std::string>& excluded_plugins)
{
    d_excluded_plugins = excluded_plugins;
}

const std::vector<std::string>& Context::excluded_plugins()
{
    return d_excluded_plugins;
}

void Context::set_conditional_variables(const std::vector<std::string>& conditionals)
{
    d_conditionals = conditionals;
}

const std::vector<std::string>& Context::conditional_variables() const
{
    return d_conditionals;
}

void Context::set_tangible_ip_address_and_port(const std::string& ip, int port)
{
    d_tangible_ip = ip;
    d_tangible_port = port;
}

void Context::set_pen_color(int color)
{
    d_pen_color = color;
}

const int Context::pen_color() const
{
    return d_pen_color;
}

const std::unordered_map<std::string, bp::object> &Context::selected_effects_by_cursor_id() const
{
    return d_selected_effects_by_id;
}

const IPhysicalEnvironment *Context::physical_environment()
{
    return d_ros;
}

void Context::update_cursor_stroke_width_by_cursor_id(const std::string &cursor_id, int stroke_width)
{
    d_ire->set_cursor_stroke_width_by_cursor_id(cursor_id, stroke_width);
}

void Context::update_cursor_stroke_color_by_cursor_id(const std::string &cursor_id, const glm::vec4 &color)
{
    d_ire->set_cursor_stroke_color_by_cursor_id(cursor_id, color);
}

void Context::push_fps(int actual, int target)
{
    if (!fps_region)
    {
        for (auto &r: uprm->regions())
            if (r->name() == "__ FPS_Counter __")
                fps_region = r.get();
    }
    else
    {
        fps_region->raw_effect().attr("__update_fps__")(actual, target);
    }
}

void Context::set_file_system_root_folder(const std::string &path)
{
    d_file_system_root_folder = path;
}

void Context::set_file_system_desktop_folder(const std::string &path)
{
    d_file_system_desktop_folder = path;
}

std::string Context::file_system_root_folder()
{
    return d_file_system_root_folder;
}

std::string Context::file_system_desktop_folder()
{
    return d_file_system_desktop_folder;
}

void Context::click_mouse(float x, float y)
{
    x+= QApplication::primaryScreen()->geometry().x();
    QWidget* target = QApplication::widgetAt(x, y);
    QPoint p = target->mapFromGlobal(QPoint(x, y));

    QTest::mouseClick(target, Qt::LeftButton, Qt::AltModifier | Qt::ControlModifier, p);
    QApplication::processEvents();
}

void Context::dbl_click_mouse(float x, float y)
{
    x+= QApplication::primaryScreen()->geometry().x();
    QWidget* target = QApplication::widgetAt(x, y);
    QPoint p = target->mapFromGlobal(QPoint(x, y));

    QTest::mouseDClick(target, Qt::LeftButton, Qt::AltModifier | Qt::ControlModifier, p);
    QApplication::processEvents();
}