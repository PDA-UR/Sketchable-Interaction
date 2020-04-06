
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

    void query_region_insertion(const std::vector<glm::vec3> &contour, const bp::object& effect, std::shared_ptr<Region>& parent, const bp::dict& kwargs=bp::dict(), const std::string& attrib_sender="", const std::string& attrib_recv="");
    void add_region(const std::vector<glm::vec3> &contour, const bp::object &effect, uint32_t region_uuid, const bp::dict& kwargs=bp::dict());

    std::vector<std::shared_ptr<Region>>& regions();
    std::map<std::string, std::vector<glm::vec3>>& partial_regions();

    void set_partial_regions(std::map<std::string, std::vector<glm::vec3>>& partials);

    void update();

private:
    std::vector<std::shared_ptr<Region>> d_regions;
    std::map<std::string, std::vector<glm::vec3>> d_partial_regions;

    void update_mouse_inputs();
    void update_region_insertions();
    void update_regions();
    void update_region_deletions();

    void activate_mouse_region_button_down(uint32_t mouse_btn);
    void deactivate_mouse_region_button_down(uint32_t mouse_btn);

    void toggle_mouse_region_wheel_scrolled(float angle_px, float angle_degrees);

    friend class SIGRunRegionManagerTest;

    std::vector<std::tuple<std::vector<glm::vec3>, bp::object, bp::dict, std::shared_ptr<Region>, std::string, std::string>> d_region_insertion_queries;
};


#endif //SITEST_REGIONMANAGER_HPP
