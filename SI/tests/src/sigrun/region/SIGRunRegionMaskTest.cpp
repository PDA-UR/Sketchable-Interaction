
#include <glm/glm.hpp>
#include <vector>
#include <sigrun/region/RegionMask.hpp>

#include "SIGRunRegionMaskTest.hpp"

TEST_F(SIGRunRegionMaskTest, construction)
{
    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

    EXPECT_NO_FATAL_FAILURE(RegionMask rm(1920, 1080, contour));
}

TEST_F(SIGRunRegionMaskTest, size_of_bitarray)
{
    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

    RegionMask rm(1920, 1080, contour);

    ASSERT_EQ(rm.size(), 1920 * 1080);
    ASSERT_EQ(sizeof(bool) * rm.size(), 1920 * 1080);
}

TEST_F(SIGRunRegionMaskTest, set_bit_index)
{
    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

    RegionMask rm(1920, 1080, contour);
    EXPECT_NO_FATAL_FAILURE(rm.set_bit(50));
}

TEST_F(SIGRunRegionMaskTest, set_bit_point)
{
    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

    RegionMask rm(1920, 1080, contour);
    glm::vec3 v(50, 0, 1);

    EXPECT_NO_FATAL_FAILURE(rm.set_bit(v));
}

TEST_F(SIGRunRegionMaskTest, test_bit_index)
{
    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

    RegionMask rm(1920, 1080, contour);
    rm.set_bit(1920 * (101 - 100) + 101 - 100);

    ASSERT_TRUE(rm[1920 * (101 - 100) + 101 - 100]);
}

TEST_F(SIGRunRegionMaskTest, test_bit_point)
{
    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

    RegionMask rm(1920, 1080, contour);
    glm::vec3 v(101, 101, 1);

    rm.set_bit(v);

    ASSERT_TRUE(rm[v]);

    glm::vec3 u(50, 0, 1);

    ASSERT_FALSE(rm[u]);
}

TEST_F(SIGRunRegionMaskTest, test_bit_point_index_equivalence)
{
    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

    RegionMask rm(1920, 1080, contour);
    glm::vec3 v(202, 202, 1);

    rm.set_bit(v);
    ASSERT_TRUE(rm[v]);

    rm.set_bit(1920 * (101 - 100) + 101);
    glm::vec3 u(101, 101, 1);

    ASSERT_TRUE(rm[1920 * (101 - 100) + 101]);
}

TEST_F(SIGRunRegionMaskTest, clear_bit_index)
{
    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

    RegionMask rm(1920, 1080, contour);
    rm.set_bit(1920 * (101 - 100) + 101 - 100);

    ASSERT_TRUE(rm[1920 * (101 - 100) + 101 - 100]);

    rm.clear_bit(1920 * (101 - 100) + 101 - 100);
    ASSERT_FALSE(rm[1920 * (101 - 100) + 101 - 100]);
}

TEST_F(SIGRunRegionMaskTest, clear_bit_point)
{
    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

    RegionMask rm(1920, 1080, contour);
    glm::vec3 v(101, 101, 1);

    rm.set_bit(v);

    ASSERT_TRUE(rm[v]);

    rm.clear_bit(v);
    ASSERT_FALSE(rm[v]);
}

TEST_F(SIGRunRegionMaskTest, clear_bit_point_index_equivalence)
{
    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

    RegionMask rm(1920, 1080, contour);
    glm::vec3 v(101, 101, 1);

    rm.set_bit(v);
    ASSERT_TRUE(rm[v]);
    ASSERT_TRUE(rm[1920 * 101 + 101]);

    rm.set_bit(1920 * (202) + 202);
    glm::vec3 u(202, 202, 1);

    ASSERT_TRUE(rm[u]);
    ASSERT_TRUE(rm[1920 * (202) + 202]);

    rm.clear_bit(1920 * (101) + 101);
    ASSERT_FALSE(rm[1920 * (101) + 101]);
    ASSERT_FALSE(rm[v]);

    rm.clear_bit(u);
    ASSERT_FALSE(rm[1920 * (202) + 202]);
    ASSERT_FALSE(rm[u]);
}

TEST_F(SIGRunRegionMaskTest, index_operator_int)
{
    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

    RegionMask rm(1920, 1080, contour);

    EXPECT_NO_FATAL_FAILURE(rm[0]);
}

TEST_F(SIGRunRegionMaskTest, index_operator_point)
{
    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

    RegionMask rm(1920, 1080, contour);
    glm::vec3 v(0, 0, 1);

    EXPECT_NO_FATAL_FAILURE(rm[v]);
}

TEST_F(SIGRunRegionMaskTest, move)
{
    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

    RegionMask rm(1920, 1080, contour);

    glm::vec2 m1(100, 100);
    glm::vec2 m2(-200, -200);

    glm::vec3 t(99, 99, 1);
    glm::vec3 u(101, 101, 1);
    glm::vec3 v(350, 350, 1);
    glm::vec3 w(599, 599, 1);
    glm::vec3 x(699, 699, 1);

    ASSERT_FALSE(rm[t]);
    ASSERT_FALSE(rm[x]);

    rm.move(m1);

    ASSERT_FALSE(rm[t]);
    ASSERT_FALSE(rm[u]);

    rm.move(m2);

    ASSERT_FALSE(rm[w]);
    ASSERT_FALSE(rm[x]);
}

TEST_F(SIGRunRegionMaskTest, scanlinefill)
{
    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2 {glm::vec3(-100, -100, 1), glm::vec3(100, 600, 1), glm::vec3(-600, -600, 1), glm::vec3(600, 100, 1)};

    EXPECT_NO_FATAL_FAILURE(scanlinefill(contour));
    EXPECT_NO_FATAL_FAILURE(scanlinefill(contour2));
}