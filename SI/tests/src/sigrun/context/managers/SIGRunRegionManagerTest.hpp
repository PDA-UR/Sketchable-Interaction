

#ifndef SITEST_SIGRUNREGIONMANAGERTEST_HPP
#define SITEST_SIGRUNREGIONMANAGERTEST_HPP

#include <gtest/gtest.h>
#include <sigrun/context/managers/RegionManager.hpp>

class SIGRunRegionManagerTest: public ::testing::Test
{
public:
    void SetUp()
    {

    }

    void TearDown()
    {

    }

    RegionManager rm;
};


#endif //SITEST_SIGRUNREGIONMANAGERTEST_HPP
