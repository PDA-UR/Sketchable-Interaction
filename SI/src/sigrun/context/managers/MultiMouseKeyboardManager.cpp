//
// Created by juergen on 21.08.23.
//

#include "MultiMouseKeyboardManager.hpp"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <sigrun/context/Context.hpp>

MultiMouseKeyboardManager::MultiMouseKeyboardManager():
        d_is_started(false)
{
    d_mmouse_coords[0].x = 0;
    d_mmouse_coords[0].y = 0;
    d_mmouse_coords[1].x = 0;
    d_mmouse_coords[1].y = 0;
    d_mmouse_coords[2].x = 0;
    d_mmouse_coords[2].y = 0;

    d_previous_mmouse_coords[0].x = 0;
    d_previous_mmouse_coords[0].y = 0;
    d_previous_mmouse_coords[1].x = 0;
    d_previous_mmouse_coords[1].y = 0;
    d_previous_mmouse_coords[2].x = 0;
    d_previous_mmouse_coords[2].y = 0;
}

MultiMouseKeyboardManager::~MultiMouseKeyboardManager() = default;

void MultiMouseKeyboardManager::start()
{
    d_max_x = Context::SIContext()->width();
    d_max_y = Context::SIContext()->height();

    for (int i = 0; i < 3; i++)
    {
        bp::dict kwargs;
        kwargs["id"] = i;
        kwargs["draw"] = "RMB";

        float x = 0;
        float y = 0;
        float w = 24 / 8;
        float h = 24 / 8;

        std::vector<glm::vec3> contour = {glm::vec3(x, y, 1), glm::vec3(x, y + h, 1), glm::vec3(x + w, y + h, 1), glm::vec3(x + w, y, 1)};

        Context::SIContext()->register_new_region_via_name(contour, SI_NAME_EFFECT_MOUSE_CURSOR, false, kwargs);
    }

    if(!d_is_started)
    {
        for(auto& [name, ev_id]: d_event_devices)
        {
            uint8_t id = ev_id[1];
            uint8_t keyboard_event_id = ev_id[2];
            uint8_t event = ev_id[0];

//            Print::print(id, event, keyboard_event_id);

            id_to_event[id] = event;
            id_to_keyboard[id] = keyboard_event_id;

            std::string device_path = "/dev/input/event" + std::to_string(event);
            id_to_fd[id] = open(device_path.c_str(), O_RDONLY | O_NONBLOCK);
            device_path = "/dev/input/event" + std::to_string(keyboard_event_id);
            id_to_kfd[id] = open(device_path.c_str(), O_RDONLY | O_NONBLOCK);

            id_to_pointing_device[id] = new QPointingDevice("MMOUSE", -1, QInputDevice::DeviceType::Mouse, QPointingDevice::PointerType::Cursor, QInputDevice::Capability::Hover | QInputDevice::Capability::Position | QInputDevice::Capability::Scroll, 1024, 3, "", QPointingDeviceUniqueId::fromNumericId(id));
        }

        d_is_started = true;
        pthread_create(&mmkthread, nullptr, reinterpret_cast<void *(*)(void *)>(MultiMouseKeyboardManager::run_helper), this);
    }
}

void *MultiMouseKeyboardManager::handle_devices()
{
    std::mutex m;
    int it = 0;

    while(d_is_started)
    {
        for(int i = 0; i < d_event_devices.size(); ++i)
        {
            struct input_event ev;

            if(id_to_fd[i] == -1)
            {
                perror("0 Error reading event");
                continue;
            }

            ssize_t bytesRead = read(id_to_fd[i], &ev, sizeof(struct input_event));

            if (bytesRead == sizeof(struct input_event))
            {
                handle_mouse_move(i, ev);
                handle_mouse_buttons(i, ev);
            }

//            bytesRead = read(id_to_kfd[i], &ev, sizeof(struct input_event));
//
//            if(id_to_kfd[i] == -1)
//            {
//                perror("0 Error reading event");
//                continue;
//            }
//
//            if (bytesRead == sizeof(struct input_event))
//            {
//                if(ev.type == EV_KEY)
//                {
////                    Print::print("TYPED", i, ev.value, ev.code);
//                }
//            }
        }
    }

    for(auto& [id, fd]: id_to_fd)
        close(fd);

    return nullptr;
}

void MultiMouseKeyboardManager::handle_mouse_move(int id, const input_event &ev)
{
    if (ev.type == EV_REL && (ev.code == REL_X || ev.code == REL_Y))
    {
        d_previous_mmouse_coords[id].x = d_mmouse_coords[id].x;
        d_previous_mmouse_coords[id].y = d_mmouse_coords[id].y;

        if (ev.code == REL_X)
        {
            d_mmouse_coords[id].x += ev.value;

            if (d_mmouse_coords[id].x < d_min_x)
                d_mmouse_coords[id].x = d_min_x;

            if (d_mmouse_coords[id].x > d_max_x - (32 * 0.602090) / 1920 * d_max_x)
                d_mmouse_coords[id].x = d_max_x - (32 * 0.602090) / 1920 * d_max_x;
        }
        else if (ev.code == REL_Y)
        {
            d_mmouse_coords[id].y += ev.value;

            if (d_mmouse_coords[id].y < d_min_y)
                d_mmouse_coords[id].y = d_min_y;

            if (d_mmouse_coords[id].y > d_max_y - (float) 32 / 1200 * d_max_y)
                d_mmouse_coords[id].y = d_max_y - (float) 32 / 1200 * d_max_y;
        }

        id_to_mouse_move_event[id] = new QMouseEvent(QEvent::Type::MouseMove, QPointF(), QPointF(d_mmouse_coords[id].x + QApplication::primaryScreen()->geometry().x(), d_mmouse_coords[id].y), Qt::NoButton, Qt::NoButton, Qt::NoModifier, id_to_pointing_device[id]);
        QApplication::postEvent(QApplication::instance(), id_to_mouse_move_event[id]);
    }
}

void MultiMouseKeyboardManager::handle_mouse_buttons(int id, const input_event &ev)
{
    if (ev.type == EV_KEY && (ev.code == BTN_LEFT || ev.code == BTN_RIGHT))
    {
        if(ev.code == BTN_LEFT)
        {

            if(ev.value == 1)
            {
                id_to_mouse_lmb_event[id] = new QMouseEvent(QEvent::Type::MouseButtonPress, QPointF(), QPointF(d_mmouse_coords[id].x + QApplication::primaryScreen()->geometry().x(), d_mmouse_coords[id].y), Qt::LeftButton, Qt::NoButton, Qt::NoModifier, id_to_pointing_device[id]);
            }
            else
            {
                id_to_mouse_lmb_event[id] = new QMouseEvent(QEvent::Type::MouseButtonRelease, QPointF(), QPointF(d_mmouse_coords[id].x + QApplication::primaryScreen()->geometry().x(), d_mmouse_coords[id].y), Qt::LeftButton, Qt::NoButton, Qt::NoModifier, id_to_pointing_device[id]);
            }

            QApplication::postEvent(QApplication::instance(), id_to_mouse_lmb_event[id]);
        }
        if(ev.code == BTN_RIGHT)
        {
            if(ev.value == 1)
            {
                id_to_mouse_rmb_event[id] = new QMouseEvent(QEvent::Type::MouseButtonPress, QPointF(), QPointF(d_mmouse_coords[id].x + QApplication::primaryScreen()->geometry().x(), d_mmouse_coords[id].y), Qt::RightButton, Qt::NoButton, Qt::NoModifier, id_to_pointing_device[id]);
            }
            else
            {
                id_to_mouse_rmb_event[id] = new QMouseEvent(QEvent::Type::MouseButtonRelease, QPointF(), QPointF(d_mmouse_coords[id].x + QApplication::primaryScreen()->geometry().x(), d_mmouse_coords[id].y), Qt::RightButton, Qt::NoButton, Qt::NoModifier, id_to_pointing_device[id]);
            }

            QApplication::postEvent(QApplication::instance(), id_to_mouse_rmb_event[id]);
        }
    }
}

bool MultiMouseKeyboardManager::is_started()
{
    return d_is_started;
}

void MultiMouseKeyboardManager::stop()
{
    d_is_started = false;
}

const glm::vec2 &MultiMouseKeyboardManager::mmouse_coords_by_id(uint8_t id) {
    return d_mmouse_coords[id];
}

const glm::vec2 &MultiMouseKeyboardManager::previous_mmouse_coords_by_id(uint8_t id) {
    return d_previous_mmouse_coords[id];
}

void MultiMouseKeyboardManager::set_event_devices(const std::unordered_map<std::string, std::vector<uint8_t>> &event_devices)
{
    d_event_devices = event_devices;
}
