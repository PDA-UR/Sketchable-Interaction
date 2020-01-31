

#include <QKeyEvent>
#include <QMouseEvent>
#include <QApplication>

#include "InputManager.hpp"
#include <sigrun/context/Context.hpp>

InputManager::~InputManager()
= default;

InputManager::InputManager():
    d_mouse_coords(0),
    d_previous_mouse_coords(0)
{SIGRUN
    d_external_objects.push_back(std::make_shared<ExternalObject>(ExternalObject::ExternalObjectType::MOUSE));
}

void InputManager::update()
{
    for(auto& [key, value]: d_key_map)
        d_previous_key_map[key] = value;

    for(auto& [key, value]: d_button_map)
        d_previous_button_map[key] = value;

    for(auto& obj: d_external_objects)
    {
        switch(obj->type())
        {
            case ExternalObject::ExternalObjectType::MOUSE:
            {
                bp::tuple args = bp::make_tuple(d_mouse_coords.x, d_mouse_coords.y);

                Q_EMIT obj->LINK_SIGNAL(UUID::uuid(), "__position__", args);
                break;
            }
        }
    }
}

void InputManager::press_key(unsigned int key_id)
{
    d_key_map[key_id] = true;
}

void InputManager::release_key(unsigned int key_id)
{
    d_key_map[key_id] = false;
}

void InputManager::press_mouse_button(unsigned int button_id)
{
    d_button_map[button_id] = true;
}

void InputManager::release_mouse_button(unsigned int button_id)
{
    d_button_map[button_id] = false;
}

bool InputManager::is_key_down(unsigned int key_id)
{
    return d_key_map.find(key_id) != d_key_map.end() ? d_key_map[key_id] : false;
}

bool InputManager::is_key_pressed(unsigned int key_id)
{
    return is_key_down(key_id) && !was_key_down(key_id);
}

bool InputManager::is_mouse_down(unsigned int button_id)
{
    return d_button_map.find(button_id) != d_button_map.end() ? d_button_map[button_id] : false;
}

bool InputManager::is_mouse_pressed(unsigned int button_id)
{
    return is_mouse_down(button_id) && !was_mouse_down(button_id);
}

bool InputManager::was_key_down(unsigned int key_id)
{
    return d_previous_key_map.find(key_id) != d_previous_key_map.end() ? d_previous_key_map[key_id] : false;
}

bool InputManager::was_mouse_down(unsigned int button_id)
{
    return d_previous_button_map.find(button_id) != d_previous_button_map.end() ? d_previous_button_map[button_id] : false;
}

const glm::vec2 &InputManager::mouse_coords() const
{
    return d_mouse_coords;
}

ExternalObject* InputManager::mouse_object()
{
    for(auto& obj: d_external_objects)
    {
        if(obj->type() == ExternalObject::ExternalObjectType::MOUSE)
            return obj.get();
    }

    return nullptr;
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

        int x = mouse_event->x();
        int y = mouse_event->y();

        d_mouse_coords.x = x;
        d_mouse_coords.y = y;

        return true;
    }
    else if(event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* mouse_event = (QMouseEvent*) event;

        int i = -1;

        switch (mouse_event->button())
        {
            case Qt::LeftButton:
                i = SI_LEFT_MOUSE_BUTTON;
                break;

            case Qt::MiddleButton:
                i = SI_MIDDLE_MOUSE_BUTTON;
                break;

            case Qt::RightButton:
                i = SI_RIGHT_MOUSE_BUTTON;
                break;
        }

        if(i == 0 || i == 1 || i == 2)
            press_mouse_button(i);

        return true;
    }
    else if(event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent* mouse_event = (QMouseEvent*) event;

        int i = -1;

        switch (mouse_event->button())
        {
            case Qt::LeftButton:
                i = SI_LEFT_MOUSE_BUTTON;
                break;

            case Qt::MiddleButton:
                i = SI_MIDDLE_MOUSE_BUTTON;
                break;

            case Qt::RightButton:
                i = SI_RIGHT_MOUSE_BUTTON;
                break;
        }

        if(i == 0 || i == 1 || i == 2)
            release_mouse_button(i);

        return true;
    }

    return QObject::eventFilter(watched, event);
}
