
#ifndef SITEST_REGIONMANAGER_HPP
#define SITEST_REGIONMANAGER_HPP

#include <vector>
#include <glm/glm.hpp>
#include <boost/python.hpp>
#include <memory>
#include <sigrun/region/Region.hpp>
#include <sigrun/context/managers/RegionManager.hpp>
#include "../../SIObject.hpp"

namespace bp = boost::python;

class RegionManager: public SIObject
{
public:
    RegionManager();
    ~RegionManager();

    void add_region(const std::vector<glm::vec3> &contour, const bp::object &effect, int region_uuid, const bp::dict& kwargs=bp::dict());
    void delete_region(const std::string& deletion_candidate_uuid);

    std::vector<std::shared_ptr<Region>>& regions();
    std::map<std::string, std::vector<glm::vec3>>& partial_regions();

    void set_partial_regions(std::map<std::string, std::vector<glm::vec3>>& partials);

    void update();

private:
    std::vector<std::shared_ptr<Region>> d_regions;
    std::map<std::string, std::vector<glm::vec3>> d_partial_regions;

    void update_via_mouse_input();

    void activate_mouse_region_button_down(int mouse_btn);
    void deactivate_mouse_region_button_down(int mouse_btn);

    void toggle_mouse_region_wheel_scrolled(float angle_px, float angle_degrees);

    friend class SIGRunRegionManagerTest;
};


#endif //SITEST_REGIONMANAGER_HPP
