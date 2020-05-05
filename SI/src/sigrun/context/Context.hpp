

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

    QMainWindow* main_window() const;

    void update();
    void enable(uint32_t what);
    void disable(uint32_t what);

    uint32_t width();
    uint32_t height();

    void register_new_region(const std::vector<glm::vec3>& contour, const std::string& uuid);
    void register_new_region_via_name(const std::vector<glm::vec3>& contour, const std::string& name);

    void spawn_folder_contents_as_regions(const std::vector<std::string>& children_paths, const std::string& uuid, const bool with_btns);
    void spawn_folder_contents_buttons_as_regions(std::shared_ptr<Region>& parent, uint32_t dir_x, uint32_t dir_y, uint32_t preview_width, uint32_t preview_height);
    void spawn_folder_contents_entries_as_regions(std::shared_ptr<Region>& parent, const std::vector<std::string>& children_paths, uint32_t dir_x, uint32_t dir_y, uint32_t dir_width, uint32_t dir_height, uint32_t preview_width, uint32_t preview_height);
    void spawn_folder_contents_entry_as_region(const std::vector<glm::vec3>& contour, std::shared_ptr<Region>& parent, const std::string& effect_type, const bp::dict& kwargs);

    const std::map<std::string, bp::object>& available_plugins() const;
    const bp::object& plugin_by_name(const std::string& name);

    std::vector<std::string> available_plugins_names();

private:
    static Context* self;
    Context();

    void add_startup_regions(const std::unordered_map<std::string, std::unique_ptr<bp::object>>& plugins);
    void add_canvas_region(const std::unordered_map<std::string, std::unique_ptr<bp::object>>& plugins);
    void add_cursor_regions(const std::unique_ptr<bp::object>& cursor_effect);
    void add_directory_region(const std::unique_ptr<bp::object>& effect);

    std::vector<std::string> d_available_plugins_names;
    std::map<std::string, bp::object> d_available_plugins;
    std::unordered_map<std::string, bp::object> d_plugins;
    std::unordered_map<std::string, bp::object> d_selected_effects_by_id;

    std::string d_mouse_uuid;
    std::string d_canvas_uuid;
    std::string d_notification_uuid;

    std::unique_ptr<RegionManager> uprm;
    std::unique_ptr<LinkingManager> uplm;
    std::unique_ptr<InputManager> upim;
    std::unique_ptr<CollisionManager> uprcm;
    std::unique_ptr<FileSystem> upfs;
    std::unique_ptr<ExternalApplicationManager> upeam;

    int d_selected_effect_index = 0;

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
