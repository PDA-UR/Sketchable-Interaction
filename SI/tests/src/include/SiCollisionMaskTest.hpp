
#ifndef SI_SICOLLISIONMASKTEST_HPP
#define SI_SICOLLISIONMASKTEST_HPP

#include <gtest/gtest.h>
#include <core/runtime/collision/CollisionMask.hpp>
#include <core/math/si_math.hpp>

class SiCollisionMaskTest : public ::testing::Test
{
public:
    virtual void SetUp()
    {

    }

    virtual void TearDown()
    {

    }

    std::vector<Vector2<float>> contour_IXI =
    {
        Vector2<float>(100, 100),
        Vector2<float>(100, 600),
        Vector2<float>(350, 375),
        Vector2<float>(600, 600),
        Vector2<float>(600, 100),
        Vector2<float>(350, 325)
    };
};

#endif //SI_SICOLLISIONMASKTEST_HPP
