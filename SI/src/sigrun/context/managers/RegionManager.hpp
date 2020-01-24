
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
    ~RegionManager();

    void add_region(const std::vector<glm::vec3>& contour, std::shared_ptr<bp::object> effect, int region_uuid);

    std::vector<std::shared_ptr<Region>>& regions();

    void update();
    RegionManager();

private:

    std::vector<std::shared_ptr<Region>> d_regions;

    friend class SIGRunRegionManagerTest;
};


#endif //SITEST_REGIONMANAGER_HPP
