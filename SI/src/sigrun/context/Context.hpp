

#ifndef SITEST_CONTEXT_HPP
#define SITEST_CONTEXT_HPP

#include <sigrun/SIObject.hpp>
#include <unordered_map>
#include <memory>
#include <QWindow>
#include <QWidget>
#include <thread>
#include <boost/python.hpp>
#include <sigrun/context/managers/RegionManager.hpp>
#include <sigrun/context/managers/TangibleManager.hpp>
#include <sigrun/context/managers/CollisionManager.hpp>
#include <sigrun/context/managers/LinkingManager.hpp>
#include <sigrun/context/managers/InputManager.hpp>
#include <sigrun/context/managers/ExternalApplicationManager.hpp>
#include <sigrun/rendering/IRenderEngine.hpp>
#include <sigrun/network/IPhysicalEnvironment.hpp>
#include <queue>
#include <QGraphicsView>
#include <sigrun/parallel/JobSystem.hpp>
#include <sigrun/context/spatial_grid/SpatialHashGrid.hpp>

#define DELAY_PYTHON_GARBAGE_COLLECTION(...) {\
    p_py_garbage_collector->attr("disable")(); \
    __VA_ARGS__ \
    p_py_garbage_collector->attr("enable")(); \
    p_py_garbage_collector->attr("collect")(2); \
}

class Context: public SIObject
{ SIGRUN


    void update_cursor_stroke_width_by_cursor_id(const std::string &cursor_id, int stroke_width);
    void update_cursor_stroke_color_by_cursor_id(const std::string &cursor_id, const glm::vec4 &color);

public:
    static Context* SIContext();

    ~Context();

    void begin(const std::unordered_map<std::string, std::unique_ptr<bp::object>>& plugins, IRenderEngine* ire, IPhysicalEnvironment* ros, int argc, char** argv);
    void end();

    RegionManager* region_manager();
    InputManager* input_manager();
    CollisionManager* collision_manager();
    LinkingManager* linking_manager();
    ExternalApplicationManager* external_application_manager();
    JobSystem<void, 512>* job_system();
    TangibleManager* tangible_manager();
    SpatialHashGrid* spatial_hash_grid();

    QGraphicsView* main_window() const;
    void set_main_window();

    void update();
    void enable(uint32_t what);
    void disable(uint32_t what);

    uint32_t width();
    uint32_t height();

    void set_effect(const std::string& target_uuid, const std::string& effect_name, bp::dict& kwargs);

    void register_new_region(const std::vector<glm::vec3>& contour, const std::string& uuid, const bp::dict& kwargs);
    void register_new_region_via_name(const std::vector<glm::vec3>& contour, const std::string& name, bool as_selector, bp::dict& kwargs);
    void register_new_region_via_type(const std::vector<glm::vec3>& contour, int type, bp::dict& kwargs);
    void register_region_via_class_object(const std::vector<glm::vec3>& contour, bp::object& clazz, bp::dict& kwargs);
    void register_new_region_from_object(const bp::object &object, const bp::dict &dict);

    void register_link_event_emission(const std::string& event_uuid, const std::string& sender_uuid, const std::string& sender_attribute, const bp::object& args);

    void register_new_application_container(uint64_t winid, uint64_t pid, const QString& window_name, const std::string& file_region_uuid);
    void unregister_external_application(const std::string& file_region_uuid);

    const std::unordered_map<std::string, bp::object>& available_plugins() const;
    const bp::object& plugin_by_name(const std::string& name);

    const std::vector<std::string>& available_plugins_names();
    const std::vector<std::string>& excluded_plugins();

    std::unordered_map<std::string, std::shared_ptr<ExternalObject>>& external_objects();

    const std::vector<std::string>& conditional_variables() const;

    void exclude_plugins(const std::vector<std::string>& excluded_plugins);

    void set_conditional_variables(const std::vector<std::string>& conditionals);

    void set_tangible_ip_address_and_port(const std::string& ip, int port);
    void set_pen_color(int color);

    const int pen_color() const;

    const std::unordered_map<std::string, bp::object>& selected_effects_by_cursor_id() const;

    const IPhysicalEnvironment* physical_environment();

    void push_fps(int actual, int target);

    void click_mouse(float x, float y);
    void dbl_click_mouse(float x, float y);

    void set_file_system_root_folder(const std::string &path);
    void set_file_system_desktop_folder(const std::string &path);
    std::string file_system_root_folder();
    std::string file_system_desktop_folder();
    std::ofstream logfile;

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

    Region* fps_region;

    std::unordered_map<std::string, std::shared_ptr<ExternalObject>> deo;

    std::queue<std::tuple<uint64_t, uint64_t, QString, std::string>> d_external_application_container_insertion_queue;
    std::queue<std::tuple<std::vector<glm::vec3>, bp::object, int32_t, bp::dict>> d_region_insertion_queue;
//    std::queue<std::tuple<std::string, std::string, std::string, bp::object>> d_link_emission_queue;

    std::vector<std::string> d_excluded_plugins;
    std::vector<std::string> d_conditionals;

    std::vector<std::string> d_available_plugins_names;
    std::unordered_map<std::string, bp::object> d_available_plugins;
    std::unordered_map<std::string, bp::object> d_plugins;
    std::unordered_map<std::string, bp::object> d_selected_effects_by_id;

    std::unique_ptr<RegionManager> uprm;
    std::unique_ptr<LinkingManager> uplm;
    std::unique_ptr<InputManager> upim;
    std::unique_ptr<CollisionManager> uprcm;
    std::unique_ptr<ExternalApplicationManager> upeam;
    std::unique_ptr<JobSystem<void, 512>> upjs;
    std::unique_ptr<TangibleManager> uptm;
    std::unique_ptr<SpatialHashGrid> upshg;

    std::string d_tangible_ip = "";
    int d_tangible_port = 0;
    int d_pen_color = 0;

    std::string d_file_system_root_folder = "";
    std::string d_file_system_desktop_folder = "";

    uint32_t s_width = 0;
    uint32_t s_height = 0;

    IRenderEngine* d_ire;
    IPhysicalEnvironment* d_ros;
    QGraphicsView* d_main_window;

    friend class Core;
    friend class SIGRunCollisionManagerTest;
    friend class SIGRunLinkingManagerTest;
    friend class SIGRunRegionTest;
    friend class PySIPySIEffectTest;
    friend class PySIPySIEffectTest;
    friend class SIGRunLinkTest;
};


#endif //SITEST_CONTEXT_HPP
