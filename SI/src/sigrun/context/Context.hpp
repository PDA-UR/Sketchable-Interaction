

#ifndef SITEST_CONTEXT_HPP
#define SITEST_CONTEXT_HPP

#include <sigrun/SIObject.hpp>
#include "Capability.hpp"
#include <unordered_map>
#include <memory>
#include <boost/python.hpp>
#include <sigrun/context/managers/RegionManager.hpp>
#include <sigrun/context/managers/CollisionManager.hpp>
#include <sigrun/context/managers/LinkingManager.hpp>
#include <sigrun/context/managers/InputManager.hpp>
#include <sigrun/rendering/IRenderEngine.hpp>
#include <sigrun/context/managers/helpers/input/FileSystem.hpp>

class Context: public SIObject
{

public:
    static Context* SIContext();

    ~Context();

    void begin(const std::unordered_map<std::string, std::unique_ptr<bp::object>>& plugins, IRenderEngine* ire, int argc, char** argv);
    void end();

    Capability* capability_manager();
    RegionManager* region_manager();
    InputManager* input_manager();
    CollisionManager* collision_manager();
    LinkingManager* linking_manager();

    void update();
    void enable(int what);
    void disable(int what);

    int width();
    int height();

    void register_new_region(const std::vector<glm::vec3>& contour, const std::string& uuid);
    void update_linking_relations(const std::vector<LinkRelation>& relations, const std::string& source);
    void remove_all_source_linking_relations(const std::string& source);
    void remove_all_partaking_linking_relations(const std::string& source);

    void spawn_folder_contents_as_regions(const std::vector<std::string>& children_paths, const std::string& uuid, const bool with_btns);
    void spawn_folder_contents_buttons_as_regions(std::shared_ptr<Region>& parent, int dir_x, int dir_y, int preview_width, int preview_height);
    void spawn_folder_contents_entries_as_regions(std::shared_ptr<Region>& parent, const std::vector<std::string>& children_paths, int dir_x, int dir_y, int dir_width, int dir_height, int preview_width, int preview_height);
    void spawn_folder_contents_entry_as_region(const std::vector<glm::vec3>& contour, std::shared_ptr<Region>& parent, const std::string& effect_type, const bp::dict& kwargs);

private:
    static Context* self;
    Context();

    void add_startup_regions(const std::unordered_map<std::string, std::unique_ptr<bp::object>>& plugins);
    void add_canvas_region(const std::unordered_map<std::string, std::unique_ptr<bp::object>>& plugins);
    void add_cursor_regions(const std::unique_ptr<bp::object>& cursor_effect);
    void add_directory_region(const std::unique_ptr<bp::object>& effect);
    void remove_linking_relations(const std::vector<LinkRelation>& relations, const std::string& source);
    void create_linking_relations(const std::vector<LinkRelation>& relations, const std::string& source);

    std::map<std::string, bp::object> d_available_plugins;
    std::unordered_map<std::string, bp::object> d_selected_effects_by_id;

    std::map<std::string, std::vector<std::shared_ptr<ILink>>> d_links_in_ctx;

    std::string d_mouse_uuid;
    std::string d_canvas_uuid;
    std::string d_notification_uuid;

    std::unique_ptr<Capability> upcm;
    std::unique_ptr<RegionManager> uprm;
    std::unique_ptr<LinkingManager> uplm;
    std::unique_ptr<InputManager> upim;
    std::unique_ptr<CollisionManager> uprcm;
    std::unique_ptr<FileSystem> upfs;

    std::string test_help = "Tag";

    int s_width = 0;
    int s_height = 0;

    IRenderEngine* d_ire;

    friend class Core;
    friend class SIGRunCollisionManagerTest;
};


#endif //SITEST_CONTEXT_HPP
