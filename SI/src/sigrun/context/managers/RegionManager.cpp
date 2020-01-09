

#include "RegionManager.hpp"
#include "../../log/Log.hpp"


RegionManager::~RegionManager()
{
}

void RegionManager::add_region(const std::vector<glm::vec3> &contour, std::shared_ptr<bp::object> effect, int region_uuid)
{
    d_regions.emplace_back(std::make_shared<Region>(contour, effect));
}

const std::vector<std::shared_ptr<Region>> &RegionManager::regions() const
{
    return d_regions;
}

void RegionManager::update(const std::vector<int> &update_vertices)
{

}

RegionManager::RegionManager()
{SIOBJECT(SIGRUN)

}