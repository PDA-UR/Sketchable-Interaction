
#ifndef SITEST_MULTIMOUSEKEYBOARDMANAGER_H
#define SITEST_MULTIMOUSEKEYBOARDMANAGER_H


#include <QPointingDevice>

class MultiMouseKeyboardManager: public SIObject
{ SIGRUN
public:
    MultiMouseKeyboardManager();
    ~MultiMouseKeyboardManager();

    void start();
    void stop();
    void* handle_devices();

    bool is_started();

    const glm::vec2& mmouse_coords_by_id(uint8_t id);
    const glm::vec2& previous_mmouse_coords_by_id(uint8_t id);

    void set_event_devices(const std::unordered_map<std::string, std::vector<uint8_t>>& event_devices);

private:
    void handle_mouse_move(int id, const struct input_event& ev);
    void handle_mouse_buttons(int id, const struct input_event& ev);

    bool d_is_started;

    pthread_t mmkthread;

    std::unordered_map<uint8_t, glm::vec2> d_mmouse_coords;
    std::unordered_map<uint8_t, glm::vec2> d_previous_mmouse_coords;

    static void* run_helper(void* obj)
    {
        MultiMouseKeyboardManager* ptm = static_cast<MultiMouseKeyboardManager*>(obj);
        return ptm->handle_devices();
    }

    std::unordered_map<std::string, std::vector<uint8_t>> d_event_devices;
    std::unordered_map<uint8_t, uint8_t> id_to_event;
    std::unordered_map<uint8_t, uint8_t> id_to_keyboard;

    std::unordered_map<uint8_t, int> id_to_fd;
    std::unordered_map<uint8_t, int> id_to_kfd;

    std::unordered_map<uint8_t, QPointingDevice*> id_to_pointing_device;
    std::unordered_map<uint8_t, QMouseEvent*> id_to_mouse_move_event;
    std::unordered_map<uint8_t, QMouseEvent*> id_to_mouse_lmb_event;
    std::unordered_map<uint8_t, QMouseEvent*> id_to_mouse_rmb_event;

    int d_min_x = 0;
    int d_min_y = 0;
    int d_max_x;
    int d_max_y;
};


#endif //SITEST_MULTIMOUSEKEYBOARDMANAGER_H
