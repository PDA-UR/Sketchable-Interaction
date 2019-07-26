

#ifndef SI_SIREGIONMANAGERTEST_HPP
#define SI_SIREGIONMANAGERTEST_HPP

#include <gtest/gtest.h>
#include <context/managers/RegionManager.hpp>

class SiRegionManagerTest : public ::testing::Test
{
public:
    virtual void SetUp()
    {

    }

    virtual void TearDown()
    {

    }

    RegionManager d_region_manager;

protected:
    void destroy()
    {
        d_region_manager.destroy();
    }
};


#endif //SI_SIREGIONMANAGERTEST_HPP
