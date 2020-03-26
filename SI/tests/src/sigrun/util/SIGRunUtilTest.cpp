

#include "SIGRunUtilTest.hpp"
#include <sigrun/util/Util.hpp>
#include <cmath>

TEST_F(SIGRunUtilTest, TO_RADIANS)
{
    ASSERT_NEAR(TO_RADIANS(90), M_PI / 2.0, 0.0001);
    ASSERT_NEAR(TO_RADIANS(45), M_PI / 4.0, 0.0001);
    ASSERT_NEAR(TO_RADIANS(60), M_PI / 3.0, 0.0001);
    ASSERT_NEAR(TO_RADIANS(30), M_PI / 6.0, 0.0001);
    ASSERT_NEAR(TO_RADIANS(10), M_PI / 18.0, 0.0001);
    ASSERT_NEAR(TO_RADIANS(180), M_PI, 0.0001);
    ASSERT_NEAR(TO_RADIANS(0), 0, 0.0001);
    ASSERT_NEAR(TO_RADIANS(360), 2 * M_PI, 0.0001);
}

TEST_F(SIGRunUtilTest, TO_DEGREES)
{
    ASSERT_NEAR(90, TO_DEGREES(M_PI / 2.0), 0.0001);
    ASSERT_NEAR(45, TO_DEGREES(M_PI / 4.0), 0.0001);
    ASSERT_NEAR(60, TO_DEGREES(M_PI / 3.0), 0.0001);
    ASSERT_NEAR(30, TO_DEGREES(M_PI / 6.0), 0.0001);
    ASSERT_NEAR(10, TO_DEGREES(M_PI / 18.0), 0.0001);
    ASSERT_NEAR(180, TO_DEGREES(M_PI), 0.0001);
    ASSERT_NEAR(0, TO_DEGREES(0), 0.0001);
    ASSERT_NEAR(360, TO_DEGREES(2 * M_PI), 0.0001);
}

TEST_F(SIGRunUtilTest, VEC_CONTAINS)
{
    std::vector<uint8_t> test {0, 1, 2, 4, 8, 16};

    ASSERT_TRUE(VEC_CONTAINS(test, 0));
    ASSERT_TRUE(VEC_CONTAINS(test, 1));
    ASSERT_TRUE(VEC_CONTAINS(test, 2));
    ASSERT_TRUE(VEC_CONTAINS(test, 4));
    ASSERT_TRUE(VEC_CONTAINS(test, 8));
    ASSERT_TRUE(VEC_CONTAINS(test, 16));
    ASSERT_FALSE(VEC_CONTAINS(test, 11));
    ASSERT_FALSE(VEC_CONTAINS(test, 5));
    ASSERT_FALSE(VEC_CONTAINS(test, 6));
    ASSERT_FALSE(VEC_CONTAINS(test, 7));
    ASSERT_FALSE(VEC_CONTAINS(test, 9));
    ASSERT_FALSE(VEC_CONTAINS(test, 10));
    ASSERT_FALSE(VEC_CONTAINS(test, 12));
    ASSERT_FALSE(VEC_CONTAINS(test, 13));
    ASSERT_FALSE(VEC_CONTAINS(test, 14));
    ASSERT_FALSE(VEC_CONTAINS(test, 15));
}

TEST_F(SIGRunUtilTest, MAP_HAS_KEY)
{
    std::map<uint8_t, uint8_t> test
    {
        {0, 0},
        {1, 1},
        {2, 2},
        {3, 3},
        {4, 4},
        {5, 5},
        {6, 6},
        {7, 7},
        {8, 8}
    };

    ASSERT_TRUE(MAP_HAS_KEY(test, 0));
    ASSERT_TRUE(MAP_HAS_KEY(test, 1));
    ASSERT_TRUE(MAP_HAS_KEY(test, 2));
    ASSERT_TRUE(MAP_HAS_KEY(test, 3));
    ASSERT_TRUE(MAP_HAS_KEY(test, 4));
    ASSERT_TRUE(MAP_HAS_KEY(test, 5));
    ASSERT_TRUE(MAP_HAS_KEY(test, 6));
    ASSERT_TRUE(MAP_HAS_KEY(test, 7));
    ASSERT_TRUE(MAP_HAS_KEY(test, 8));
    ASSERT_FALSE(MAP_HAS_KEY(test, 9));
    ASSERT_FALSE(MAP_HAS_KEY(test, -1));
    ASSERT_FALSE(MAP_HAS_KEY(test, 10));
    ASSERT_FALSE(MAP_HAS_KEY(test, 11));
}

TEST_F(SIGRunUtilTest, HEX_STRING_TO_UINT64)
{
    std::string hex = "aabbccdd";
    ASSERT_EQ(2864434397, HEX_STRING_TO_UINT64(hex));
}