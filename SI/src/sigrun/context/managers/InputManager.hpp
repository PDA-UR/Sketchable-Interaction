

#ifndef SITEST_INPUTMANAGER_HPP
#define SITEST_INPUTMANAGER_HPP

#include <sigrun/SIObject.hpp>
#include <sigrun/log/Log.hpp>
#include <sigrun/region/Region.hpp>
#include <QObject>
#include <unordered_map>
#include <QEvent>
#include <glm/glm.hpp>
#include "helpers/input/ExternalObject.hpp"

class InputManager: public QObject, public SIObject
{Q_OBJECT SIGRUN

#define SI_LEFT_MOUSE_BUTTON 0
#define SI_RIGHT_MOUSE_BUTTON 1
#define SI_MIDDLE_MOUSE_BUTTON 2
#define SI_KEY_A Qt::Key_A
#define SI_KEY_B Qt::Key_B
#define SI_KEY_C Qt::Key_C
#define SI_KEY_D Qt::Key_D
#define SI_KEY_E Qt::Key_E
#define SI_KEY_F Qt::Key_F
#define SI_KEY_G Qt::Key_G
#define SI_KEY_H Qt::Key_H
#define SI_KEY_I Qt::Key_I
#define SI_KEY_J Qt::Key_J
#define SI_KEY_K Qt::Key_K
#define SI_KEY_L Qt::Key_L
#define SI_KEY_M Qt::Key_M
#define SI_KEY_N Qt::Key_N
#define SI_KEY_O Qt::Key_O
#define SI_KEY_P Qt::Key_P
#define SI_KEY_Q Qt::Key_Q
#define SI_KEY_R Qt::Key_R
#define SI_KEY_S Qt::Key_S
#define SI_KEY_T Qt::Key_T
#define SI_KEY_U Qt::Key_U
#define SI_KEY_V Qt::Key_V
#define SI_KEY_W Qt::Key_W
#define SI_KEY_X Qt::Key_X
#define SI_KEY_Y Qt::Key_Y
#define SI_KEY_Z Qt::Key_Z
#define SI_KEY_1 Qt::Key_1
#define SI_KEY_2 Qt::Key_2
#define SI_KEY_3 Qt::Key_3
#define SI_KEY_4 Qt::Key_4
#define SI_KEY_5 Qt::Key_5
#define SI_KEY_6 Qt::Key_6
#define SI_KEY_7 Qt::Key_7
#define SI_KEY_8 Qt::Key_8
#define SI_KEY_9 Qt::Key_9
#define SI_KEY_0 Qt::Key_0
#define SI_KEY_UP Qt::Key_Up
#define SI_KEY_DOWN Qt::Key_Down
#define SI_KEY_LEFT Qt::Key_Left
#define SI_KEY_RIGHT Qt::Key_Right
#define SI_KEY_SHIFT_L Qt::Key_Shift
#define SI_KEY_SHIFT_R Qt::Key_Shift
#define SI_KEY_CTRL_L Qt::Key_Control
#define SI_KEY_CTRL_R Qt::Key_Control

public:
    struct MouseWheelAngles
    {
        float px;
        float degrees;
    };

    InputManager();
    ~InputManager();

    bool eventFilter(QObject *watched, QEvent *event) override;

    void update();

    void press_key(uint32_t key_id);
    void release_key(uint32_t key_id);
    void press_mouse_button(uint32_t button_id);
    void release_mouse_button(uint32_t button_id);
    bool is_key_down(uint32_t key_id);
    bool is_key_pressed(uint32_t key_id);
    bool is_mouse_down(uint32_t button_id);
    bool is_mouse_pressed(uint32_t button_id);

    const glm::vec2& mouse_coords() const;

    std::unordered_map<std::string, std::shared_ptr<ExternalObject>>& external_objects();

    void register_external_application(std::shared_ptr<Region>& reg, QWidget* window, uint64_t pid);

    const MouseWheelAngles mouse_wheel_angles();

private:
    bool was_key_down(uint32_t key_id);
    bool was_mouse_down(uint32_t button_id);

    std::unordered_map<uint32_t, bool> d_key_map;
    std::unordered_map<uint32_t, bool> d_previous_key_map;

    std::unordered_map<uint32_t, bool> d_button_map;
    std::unordered_map<uint32_t, bool> d_previous_button_map;

    glm::vec2 d_mouse_coords;
    glm::vec2 d_previous_mouse_coords;

    std::unordered_map<std::string, std::shared_ptr<ExternalObject>> deo;

    float d_mouse_wheel_angle_in_px;
};


#endif //SITEST_INPUTMANAGER_HPP
