

#ifndef SITEST_CONTEXT_HPP
#define SITEST_CONTEXT_HPP

#include <sigrun/SIObject.hpp>
#include <unordered_map>
#include <memory>
#include <QWindow>
#include <QWidget>
#include <boost/python.hpp>
#include <sigrun/context/managers/RegionManager.hpp>
#include <sigrun/context/managers/CollisionManager.hpp>
#include <sigrun/context/managers/LinkingManager.hpp>
#include <sigrun/context/managers/InputManager.hpp>
#include <sigrun/context/managers/ExternalApplicationManager.hpp>
#include <sigrun/rendering/IRenderEngine.hpp>
#include <sigrun/context/managers/helpers/input/FileSystem.hpp>
#include <queue>
#include <sigrun/parallel/JobSystem.hpp>

#define DELAY_PYTHON_GARBAGE_COLLECTION(...) {\
    p_py_garbage_collector->attr("disable")(); \
    __VA_ARGS__ \
    p_py_garbage_collector->attr("enable")(); \
    p_py_garbage_collector->attr("collect")(2); \
}

class Context: public SIObject
{ SIGRUN

public:
    static Context* SIContext();

    ~Context();

    void begin(const std::unordered_map<std::string, std::unique_ptr<bp::object>>& plugins, IRenderEngine* ire, int argc, char** argv);
    void end();

    RegionManager* region_manager();
    InputManager* input_manager();
    CollisionManager* collision_manager();
    LinkingManager* linking_manager();
    ExternalApplicationManager* external_application_manager();
    JobSystem<void, 512>* job_system();

    QMainWindow* main_window() const;
    void set_main_window();

    void update();
    void enable(uint32_t what);
    void disable(uint32_t what);

    uint32_t width();
    uint32_t height();

    void set_effect(const std::string& target_uuid, const std::string& effect_name, const std::string& effect_display_name, bp::dict& kwargs);

    void register_new_region(const std::vector<glm::vec3>& contour, const std::string& uuid);
    void register_new_region_via_name(const std::vector<glm::vec3>& contour, const std::string& name, bool as_selector, bp::dict& kwargs);
    void register_new_region_via_type(const std::vector<glm::vec3>& contour, int type, bp::dict& kwargs);
    void register_region_via_class_object(const std::vector<glm::vec3>& contour, bp::object& clazz, bp::dict& kwargs);

    void register_link_event_emission(const std::string& event_uuid, const std::string& sender_uuid, const std::string& sender_attribute, const bp::object& args);

    void register_new_application_container(uint64_t winid, uint64_t pid, const QString& window_name, const std::string& file_region_uuid);
    void unregister_external_application(const std::string& file_region_uuid);

    const std::unordered_map<std::string, bp::object>& available_plugins() const;
    const bp::object& plugin_by_name(const std::string& name);

    const std::vector<std::string>& available_plugins_names();

    std::unordered_map<std::string, std::shared_ptr<ExternalObject>>& external_objects();

    void set_message(const std::string& msg);

private:
    static Context* self;
    Context();

    bp::object* p_py_garbage_collector;

    void perform_external_object_update();
    void perform_mouse_update(std::unordered_map<std::string, std::shared_ptr<ExternalObject>>::iterator& it);
    void perform_external_application_update(std::unordered_map<std::string, std::shared_ptr<ExternalObject>>::iterator& it);
    void perform_external_application_registration();
    void perform_region_insertion();
    void perform_link_events();
    void perform_input_update();
    void perform_collision_update();

    std::unordered_map<std::string, std::shared_ptr<ExternalObject>> deo;

    std::queue<std::tuple<uint64_t, uint64_t, QString, std::string>> d_external_application_container_insertion_queue;
    std::queue<std::tuple<std::vector<glm::vec3>, bp::object, int32_t, bp::dict>> d_region_insertion_queue;
    std::queue<std::tuple<std::string, std::string, std::string, bp::object>> d_link_emission_queue;

    std::vector<std::string> d_available_plugins_names;
    std::unordered_map<std::string, bp::object> d_available_plugins;
    std::unordered_map<std::string, bp::object> d_plugins;
    std::unordered_map<std::string, bp::object> d_selected_effects_by_id;

    std::unique_ptr<RegionManager> uprm;
    std::unique_ptr<LinkingManager> uplm;
    std::unique_ptr<InputManager> upim;
    std::unique_ptr<CollisionManager> uprcm;
    std::unique_ptr<FileSystem> upfs;
    std::unique_ptr<ExternalApplicationManager> upeam;
    std::unique_ptr<JobSystem<void, 512>> upjs;

    uint32_t s_width = 0;
    uint32_t s_height = 0;

    IRenderEngine* d_ire;
    QMainWindow* d_main_window;

    friend class Core;
    friend class SIGRunCollisionManagerTest;
    friend class SIGRunLinkingManagerTest;
    friend class PySIPySIEffectTest;
};


#endif //SITEST_CONTEXT_HPP
