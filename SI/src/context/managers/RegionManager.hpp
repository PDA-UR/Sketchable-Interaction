

#ifndef SI_REGIONMANAGER_HPP
#define SI_REGIONMANAGER_HPP

#include <core/region/Region.hpp>

class RegionManager
{
public:
    void add_region(Region r);
    void remove_region(int id);
private:
    RegionManager();
    ~RegionManager();

    std::vector<Region> d_regions;
};


#endif //SI_REGIONMANAGER_HPP
