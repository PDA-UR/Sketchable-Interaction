

#ifndef SI_REGIONMANAGER_HPP
#define SI_REGIONMANAGER_HPP

#include <core/runtime/region/Region.hpp>

class RegionManager
{
public:
    void add_region(const Region& r);
    void remove_region(int id);

    int num_regions();

    const std::vector<Region>& regions() const;
private:
    RegionManager();
    ~RegionManager();

    void destroy();

    std::vector<Region> d_regions;

    friend class Context;
    friend class SiRegionManagerTest;
};


#endif //SI_REGIONMANAGER_HPP
