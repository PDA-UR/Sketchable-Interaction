
#ifndef SITEST_REGIONMANAGER_HPP
#define SITEST_REGIONMANAGER_HPP

#include <vector>
#include <glm/glm.hpp>
#include <boost/python.hpp>
#include <memory>
#include <sigrun/region/Region.hpp>
#include <sigrun/context/managers/RegionManager.hpp>
#include "../../SIObject.hpp"
#include <cstdarg>
#include <sigrun/context/managers/helpers/linking/Link.hpp>

namespace bp = boost::python;

class RegionManager: public SIObject
{ SIGRUN
public:
    RegionManager();
    ~RegionManager();

    void add_region(const std::vector<glm::vec3> &contour, const bp::object &effect, const bp::dict& kwargs=bp::dict());
    void add_region(const bp::object& o, const bp::dict& qml);

    std::vector<std::shared_ptr<Region>>& regions();
    std::unordered_map<std::string, std::vector<glm::vec3>>& partial_regions();

    void set_partial_regions(const std::unordered_map<std::string, std::vector<glm::vec3>>& partials);

    void update();

private:
    std::vector<std::shared_ptr<Region>> d_regions;
    std::unordered_map<std::string, std::vector<glm::vec3>> d_partial_regions;

    void update_key_inputs();
    void update_mouse_inputs();
    void update_regions();
    void update_region_deletions();

    void activate_mouse_region_button_down(Region* cursor, uint32_t mouse_btn);
    void deactivate_mouse_region_button_down(Region* cursor, uint32_t mouse_btn);
    void toggle_mouse_region_double_click(Region* cursor, bool toggle);
    void toggle_mouse_region_wheel_scrolled(Region* cursor, float angle_px, float angle_degrees);

    bool d_previous_double_click_toggle = false;


    friend class SIGRunRegionManagerTest;
};


#endif //SITEST_REGIONMANAGER_HPP
