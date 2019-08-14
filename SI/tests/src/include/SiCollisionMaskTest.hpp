
#ifndef SI_SICOLLISIONMASKTEST_HPP
#define SI_SICOLLISIONMASKTEST_HPP

#include <gtest/gtest.h>
#include <core/runtime_core/collision/CollisionMask.hpp>
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

    std::vector<glm::vec2> contour_IXI =
    {
            glm::vec2(100, 100),
            glm::vec2(100, 600),
            glm::vec2(350, 375),
            glm::vec2(600, 600),
            glm::vec2(600, 100),
            glm::vec2(350, 325)
    };
};

#endif //SI_SICOLLISIONMASKTEST_HPP
