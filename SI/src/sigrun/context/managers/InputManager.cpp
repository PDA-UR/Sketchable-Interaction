

#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QApplication>
#include <execution>

#include "InputManager.hpp"
#include <sigrun/context/Context.hpp>
#include <csignal>
#include <cstdlib>

InputManager::~InputManager()
{

}

InputManager::InputManager():
    d_mouse_coords(0),
    d_previous_mouse_coords(0),
    d_mouse_wheel_angle_in_px(0)
{

}

void InputManager::update()
{
    std::for_each(std::execution::par_unseq, d_key_map.begin(), d_key_map.end(), [&](auto& pair)
    {
        d_previous_key_map[pair.first] = pair.second;
    });

    std::for_each(std::execution::par_unseq, d_button_map.begin(), d_button_map.end(), [&](auto& pair)
    {
        d_previous_button_map[pair.first] = pair.second;
    });

    for(auto it = deo.begin(); it != deo.end();)
    {
        switch(it->second->type())
        {
            case ExternalObject::ExternalObjectType::MOUSE:
            {
                bp::tuple args = bp::make_tuple(d_previous_mouse_coords.x, d_previous_mouse_coords.y, d_mouse_coords.x, d_mouse_coords.y);

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

                    auto it2 = std::find_if(std::execution::par_unseq, regions.begin(), regions.end(), [&](auto& region)
                    {
                       return region->uuid() == it->first;
                    });

                    if(it2 != regions.end())
                    {
                        HANDLE_PYTHON_CALL(
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
}

const InputManager::MouseWheelAngles InputManager::mouse_wheel_angles()
{
    MouseWheelAngles mwa;
    mwa.px = d_mouse_wheel_angle_in_px;
    mwa.degrees = d_mouse_wheel_angle_in_px / 8; // QT5 documentation states that dividing by 8 yields angle in degrees ¯\_(ツ)_/¯

    d_mouse_wheel_angle_in_px = 0.0;

    return mwa;
}

void InputManager::press_key(uint32_t key_id)
{
    d_key_map[key_id] = true;
}

void InputManager::release_key(uint32_t key_id)
{
    d_key_map[key_id] = false;
}

void InputManager::press_mouse_button(uint32_t button_id)
{
    d_button_map[button_id] = true;
}

void InputManager::release_mouse_button(uint32_t button_id)
{
    d_button_map[button_id] = false;
}

bool InputManager::is_key_down(uint32_t key_id)
{
    return d_key_map.find(key_id) != d_key_map.end() ? d_key_map[key_id] : false;
}

bool InputManager::is_key_pressed(uint32_t key_id)
{
    return is_key_down(key_id) && !was_key_down(key_id);
}

bool InputManager::is_mouse_down(uint32_t button_id)
{
    return d_button_map.find(button_id) != d_button_map.end() ? d_button_map[button_id] : false;
}

bool InputManager::is_mouse_pressed(uint32_t button_id)
{
    return is_mouse_down(button_id) && !was_mouse_down(button_id);
}

bool InputManager::was_key_down(uint32_t key_id)
{
    return d_previous_key_map.find(key_id) != d_previous_key_map.end() ? d_previous_key_map[key_id] : false;
}

bool InputManager::was_mouse_down(uint32_t button_id)
{
    return d_previous_button_map.find(button_id) != d_previous_button_map.end() ? d_previous_button_map[button_id] : false;
}

const glm::vec2 &InputManager::mouse_coords() const
{
    return d_mouse_coords;
}

bool InputManager::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent *key_event = (QKeyEvent*)event;

        switch(key_event->key())
        {
            case Qt::Key_Escape:
                Context::SIContext()->end();
                break;

            default:
                press_key(key_event->key());
                break;
        }

        return true;
    }
    else if(event->type() == QEvent::KeyRelease)
    {
        QKeyEvent *key_event = (QKeyEvent*)event;

        switch(key_event->key())
        {
            case Qt::Key_Escape:
                break;

            default:
                release_key(key_event->key());
                break;
        }

        return true;

    }
    else if(event->type() == QEvent::MouseMove)
    {
        QMouseEvent* mouse_event = (QMouseEvent*) event;

        uint32_t x = mouse_event->x();
        uint32_t y = mouse_event->y();

        d_mouse_coords.x = x;
        d_mouse_coords.y = y;

        return true;
    }
    else if(event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* mouse_event = (QMouseEvent*) event;

        switch (mouse_event->button())
        {
            case Qt::LeftButton:
                press_mouse_button(SI_LEFT_MOUSE_BUTTON);
                break;

            case Qt::MiddleButton:
                press_mouse_button(SI_MIDDLE_MOUSE_BUTTON);
                break;

            case Qt::RightButton:
                press_mouse_button(SI_RIGHT_MOUSE_BUTTON);
                break;
        }

        return true;
    }
    else if(event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent* mouse_event = (QMouseEvent*) event;

        switch (mouse_event->button())
        {
            case Qt::LeftButton:
                release_mouse_button(SI_LEFT_MOUSE_BUTTON);
                break;

            case Qt::MiddleButton:
                release_mouse_button(SI_MIDDLE_MOUSE_BUTTON);
                break;

            case Qt::RightButton:
                release_mouse_button(SI_RIGHT_MOUSE_BUTTON);
                break;
        }
        return true;
    }
    else if(event->type() == QEvent::Wheel)
    {
        QWheelEvent* wheel_event = (QWheelEvent*) event;

        d_mouse_wheel_angle_in_px = wheel_event->angleDelta().y();

        return true;
    }

    return QObject::eventFilter(watched, event);
}

std::unordered_map<std::string, std::shared_ptr<ExternalObject>>& InputManager::external_objects()
{
    return deo;
}

void InputManager::register_external_application(const std::string& file_uuid, QWidget *window, uint64_t pid)
{
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
    kwargs[SI_LINUX_PID] = pid;

    Context::SIContext()->region_manager()->add_region(contour, Context::SIContext()->plugin_by_name(SI_NAME_EFFECT_CONTAINER), 0);

    auto& container = Context::SIContext()->region_manager()->regions().back();

    deo[container->uuid()] = std::make_shared<ExternalObject>(ExternalObject::ExternalObjectType::APPLICATION);
    deo[container->uuid()]->embedded_object.external_application.window = window;
    deo[container->uuid()]->embedded_object.external_application.pid = pid;
    deo[container->uuid()]->embedded_object.external_application.file_uuid = strdup(file_uuid.c_str());

    Context::SIContext()->linking_manager()->add_link(deo[container->uuid()], container, SI_CAPABILITY_LINK_POSITION, SI_CAPABILITY_LINK_POSITION);
}

void InputManager::unregister_external_application(const std::string& file_uuid)
{
    auto it = std::find_if(std::execution::par_unseq, deo.begin(), deo.end(), [&file_uuid](const auto& pair) -> bool
    {
        return std::get<1>(pair)->type() == ExternalObject::APPLICATION && std::get<1>(pair)->embedded_object.external_application.file_uuid == file_uuid;
    });

    if(it != deo.end())
    {
        Context::SIContext()->linking_manager()->remove_link(it->second, Context::SIContext()->region_manager()->region_by_uuid(it->first), SI_CAPABILITY_LINK_POSITION, SI_CAPABILITY_LINK_POSITION);
        kill(deo[it->first]->embedded_object.external_application.pid, SIGTERM);
    }
}
