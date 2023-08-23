

#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QApplication>
#include <execution>
#include <QApplication>
#include <QPointerEvent>
#include <QScreen>
#include <QtDebug>

#include "InputManager.hpp"
#include <sigrun/context/Context.hpp>
#include <csignal>
#include <cstdlib>

InputManager::~InputManager()
{

}

InputManager::InputManager(): d_mouse_wheel_angle_in_px(0)
{
    d_mouse_coords[0] = glm::vec2(0);
    d_mouse_coords[1] = glm::vec2(0);
    d_mouse_coords[2] = glm::vec2(0);

    d_previous_mouse_coords[0] = glm::vec2(0);
    d_previous_mouse_coords[1] = glm::vec2(0);
    d_previous_mouse_coords[2] = glm::vec2(0);
}

void InputManager::update()
{
    for(const auto& [k, v]: d_key_map)
        d_previous_key_map[k] = v;

    for(const auto& [id, map]: d_button_map)
    {
        for(auto& [btn_id, is_pressed]: map)
        {
            d_previous_button_map[id][btn_id] = is_pressed;
        }
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

void InputManager::press_mouse_button(uint32_t button_id, uint8_t mid)
{
    d_button_map[mid][button_id] = true;
}

void InputManager::release_mouse_button(uint32_t button_id, uint8_t mid)
{
    d_button_map[mid][button_id] = false;
}

bool InputManager::is_key_down(uint32_t key_id)
{
    return d_key_map.find(key_id) != d_key_map.end() ? d_key_map[key_id] : false;
}

bool InputManager::is_key_pressed(uint32_t key_id)
{
    return is_key_down(key_id) && !was_key_down(key_id);
}

bool InputManager::is_mouse_down(uint32_t button_id, uint8_t mid)
{
    return d_button_map[mid].find(button_id) != d_button_map[mid].end() ? d_button_map[mid][button_id] : false;
}

const bool InputManager::is_double_click()
{
    return false; // d_button_map.find(SI_DOUBLE_CLICK) != d_button_map.end() ? d_button_map[SI_DOUBLE_CLICK] : false && !(d_previous_button_map.find(SI_DOUBLE_CLICK) != d_previous_button_map.end() ? d_previous_button_map[SI_DOUBLE_CLICK] : false);
}

bool InputManager::is_mouse_pressed(uint32_t button_id, uint8_t mid)
{
    return is_mouse_down(button_id, mid) && !was_mouse_down(button_id, mid);
}

bool InputManager::was_key_down(uint32_t key_id)
{
    return d_previous_key_map.find(key_id) != d_previous_key_map.end() ? d_previous_key_map[key_id] : false;
}

bool InputManager::was_mouse_down(uint32_t button_id, uint8_t mid)
{
    return d_previous_button_map[mid].find(button_id) != d_previous_button_map[mid].end() ? d_previous_button_map[mid][button_id] : false;
}

glm::vec2 &InputManager::mouse_coords(uint8_t id)
{
    return d_mouse_coords[id];
}

glm::vec2 &InputManager::previous_mouse_coords(uint8_t id)
{
    return d_previous_mouse_coords[id];
}

bool InputManager::eventFilter(QObject *watched, QEvent *event)
{
    d_is_double_click = false;

    if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent *key_event = (QKeyEvent*)event;

        switch(key_event->key())
        {
            case Qt::Key_Escape:
                Context::SIContext()->end();

                return true;

            default:
                press_key(key_event->key());
                break;
        }

        return QObject::eventFilter(watched, event);
    }

    if(event->type() == QEvent::KeyRelease)
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

        return QObject::eventFilter(watched, event);
    }

    if(event->type() == QEvent::MouseMove)
    {
        QMouseEvent* mouse_event = (QMouseEvent*) event;

        int numeric_id = mouse_event->pointingDevice()->uniqueId().numericId();

        uint32_t x = mouse_event->globalPosition().x() - QApplication::primaryScreen()->geometry().x();
        uint32_t y = mouse_event->globalPosition().y();

        d_previous_mouse_coords[numeric_id].x = d_mouse_coords[numeric_id].x;
        d_previous_mouse_coords[numeric_id].y = d_mouse_coords[numeric_id].y;

        d_mouse_coords[numeric_id].x = x;
        d_mouse_coords[numeric_id].y = y;
        return true;

//        QMouseEvent* mouse_event = (QMouseEvent*) event;
//
//        uint32_t x = mouse_event->globalPosition().x() - QApplication::primaryScreen()->geometry().x();
//        uint32_t y = mouse_event->globalPosition().y();
//
//        d_previous_mouse_coords.x = d_mouse_coords.x;
//        d_previous_mouse_coords.y = d_mouse_coords.y;
//
//        d_mouse_coords.x = x;
//        d_mouse_coords.y = y;
//
//        return false;
    }

//    if(event->type() == QEvent::MouseButtonDblClick)
//    {
//        press_mouse_button(SI_DOUBLE_CLICK);
//    }
//    else
//    {
//        release_mouse_button(SI_DOUBLE_CLICK);
//    }

    if(event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* mouse_event = (QMouseEvent*) event;
        int numeric_id = mouse_event->pointingDevice()->uniqueId().numericId();

        switch (mouse_event->button())
        {
            case Qt::LeftButton:
                press_mouse_button(SI_LEFT_MOUSE_BUTTON, numeric_id);
                break;
            case Qt::MiddleButton:
                press_mouse_button(SI_MIDDLE_MOUSE_BUTTON, numeric_id);
                break;
            case Qt::RightButton:
                press_mouse_button(SI_RIGHT_MOUSE_BUTTON, numeric_id);
                break;
        }

        return true;

//        switch (mouse_event->button())
//        {
//            case Qt::LeftButton:
//            {
//                if(mouse_event->modifiers() & (Qt::AltModifier | Qt::ControlModifier))
//                {
//                }
//                else
//                {
////                    long start = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count();
////                    Context::SIContext()->logfile << "start:" << start << '\n';
//                    press_mouse_button(SI_LEFT_MOUSE_BUTTON);
//                }
//            }
//            break;
//
//            case Qt::MiddleButton:
//                press_mouse_button(SI_MIDDLE_MOUSE_BUTTON);
//                break;
//
//            case Qt::RightButton:
//                press_mouse_button(SI_RIGHT_MOUSE_BUTTON);
//                break;
//        }
    }

    if(event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent* mouse_event = (QMouseEvent*) event;
        int numeric_id = mouse_event->pointingDevice()->uniqueId().numericId();

        switch (mouse_event->button())
        {
            case Qt::LeftButton:
                release_mouse_button(SI_LEFT_MOUSE_BUTTON, numeric_id);
                break;

            case Qt::MiddleButton:
                release_mouse_button(SI_MIDDLE_MOUSE_BUTTON, numeric_id);
                break;

            case Qt::RightButton:
                release_mouse_button(SI_RIGHT_MOUSE_BUTTON, numeric_id);
                break;
        }

        return true;

//        QMouseEvent* mouse_event = (QMouseEvent*) event;
//
//        switch (mouse_event->button())
//        {
//            case Qt::LeftButton:
//                release_mouse_button(SI_LEFT_MOUSE_BUTTON);
//                break;
//
//            case Qt::MiddleButton:
//                release_mouse_button(SI_MIDDLE_MOUSE_BUTTON);
//                break;
//
//            case Qt::RightButton:
//                release_mouse_button(SI_RIGHT_MOUSE_BUTTON);
//                break;
//        }
    }


    if(event->type() == QEvent::Wheel)
    {
        QWheelEvent* wheel_event = (QWheelEvent*) event;

        d_mouse_wheel_angle_in_px = wheel_event->angleDelta().y();
    }

    return QObject::eventFilter(watched, event);
}
