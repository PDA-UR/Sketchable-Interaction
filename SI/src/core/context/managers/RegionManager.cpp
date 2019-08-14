

#include "RegionManager.hpp"

namespace SI
{
    void RegionManager::add_region(const Region &r)
    {
        d_regions.push_back(r);
    }

    void RegionManager::remove_region(int id)
    {
        if (d_regions.size() == 1)
            d_regions.clear();
        else
            for (int i = d_regions.size() - 1; i > -1; i--)
                if (d_regions[i].id() == id)
                {
                    d_regions.erase(d_regions.begin() + i);
                }
    }

    int RegionManager::num_regions()
    {
        return d_regions.size();
    }

    std::vector<Region> &RegionManager::regions()
    {
        return d_regions;
    }

    void RegionManager::destroy()
    {
        d_regions.clear();
    }

    RegionManager::RegionManager()
    {

    }

    RegionManager::~RegionManager()
    {
        destroy();
    }
}