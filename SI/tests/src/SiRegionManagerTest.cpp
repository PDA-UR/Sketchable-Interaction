

#include <core/debug/Print.hpp>
#include "include/SiRegionManagerTest.hpp"

namespace SI
{
    TEST_F(SiRegionManagerTest, num_regions)
    {
        EXPECT_NO_FATAL_FAILURE(d_region_manager.num_regions());
        EXPECT_EQ(0, d_region_manager.num_regions());

        Region r(2);

        EXPECT_NO_FATAL_FAILURE(d_region_manager.add_region(r));

        EXPECT_EQ(1, d_region_manager.num_regions());
    }

    TEST_F(SiRegionManagerTest, regions)
    {
        Region r1(0);
        Region r2(1);
        Region r3(2);

        EXPECT_NO_FATAL_FAILURE(d_region_manager.add_region(r1));
        EXPECT_NO_FATAL_FAILURE(d_region_manager.add_region(r2));
        EXPECT_NO_FATAL_FAILURE(d_region_manager.add_region(r3));

        EXPECT_NO_FATAL_FAILURE(d_region_manager.regions());
        EXPECT_EQ(3, d_region_manager.num_regions());
    }

    TEST_F(SiRegionManagerTest, add_region)
    {
        Region r1(0);
        Region r2(1);
        Region r3(2);

        EXPECT_NO_FATAL_FAILURE(d_region_manager.add_region(r1));
        EXPECT_NO_FATAL_FAILURE(d_region_manager.add_region(r2));
        EXPECT_NO_FATAL_FAILURE(d_region_manager.add_region(r3));

        EXPECT_EQ(3, d_region_manager.num_regions());
        EXPECT_EQ(0, d_region_manager.regions()[0].id());
        EXPECT_EQ(1, d_region_manager.regions()[1].id());
        EXPECT_EQ(2, d_region_manager.regions()[2].id());
    }


    TEST_F(SiRegionManagerTest, remove_region)
    {
        Region r1(0);
        Region r2(1);
        Region r3(2);

        EXPECT_NO_FATAL_FAILURE(d_region_manager.add_region(r1));
        EXPECT_NO_FATAL_FAILURE(d_region_manager.add_region(r2));
        EXPECT_NO_FATAL_FAILURE(d_region_manager.add_region(r3));

        EXPECT_EQ(3, d_region_manager.num_regions());
        EXPECT_EQ(0, d_region_manager.regions()[0].id());
        EXPECT_EQ(1, d_region_manager.regions()[1].id());
        EXPECT_EQ(2, d_region_manager.regions()[2].id());

        EXPECT_NO_FATAL_FAILURE(d_region_manager.remove_region(0));

        EXPECT_EQ(2, d_region_manager.num_regions());
        EXPECT_EQ(1, d_region_manager.regions()[0].id());
        EXPECT_EQ(2, d_region_manager.regions()[1].id());
        EXPECT_NO_FATAL_FAILURE(d_region_manager.remove_region(0));
        EXPECT_NO_FATAL_FAILURE(d_region_manager.remove_region(1));
        EXPECT_EQ(1, d_region_manager.num_regions());
        EXPECT_EQ(2, d_region_manager.regions()[0].id());
        EXPECT_NO_FATAL_FAILURE(d_region_manager.remove_region(2));
        EXPECT_EQ(0, d_region_manager.num_regions());
    }

    TEST_F(SiRegionManagerTest, destroy)
    {
        destroy();
    }

}