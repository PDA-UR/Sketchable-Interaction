
#ifndef SITEST_REGIONMANAGER_HPP
#define SITEST_REGIONMANAGER_HPP

#include <vector>
#include <glm/glm.hpp>
#include <boost/python.hpp>
#include <memory>
#include <sigrun/region/Region.hpp>
#include <sigrun/context/managers/RegionManager.hpp>

class RegionManager;

namespace bp = boost::python;

class RegionManager
{
public:
    ~RegionManager();

    void add_region(const std::vector<glm::vec3>& contour, std::shared_ptr<bp::object> effect, int region_uuid);

    const std::vector<std::shared_ptr<Region>>& regions() const;

    void update(const std::vector<int>& update_vertices);

private:
    RegionManager();

    std::vector<std::shared_ptr<Region>> d_regions;

    friend class Context;
    friend class SIGRunRegionManagerTest;
};


#endif //SITEST_REGIONMANAGER_HPP
