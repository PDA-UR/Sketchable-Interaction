
#include <sigrun/region/RegionTransform.hpp>
#include "SIGRunRegionTransformTest.hpp"
#include <execution>

TEST_F(SIGRunRegionTransformTest, construction)
{
    EXPECT_NO_FATAL_FAILURE(RegionTransform rt);
}

TEST_F(SIGRunRegionTransformTest, initialize_identity)
{
    RegionTransform rt;

    for(int i = 0; i < 3; ++i)
    {
        for (int k = 0; k < 3; ++k)
        {
            bool value = rt.transform()[i][k];

            if(i == k)
                ASSERT_TRUE(value);
            else
                ASSERT_FALSE(value);
        }
    }
}

TEST_F(SIGRunRegionTransformTest, indexing_operator)
{
    RegionTransform rt;
    for(int i = 0; i < 3; ++i)
    {
        for (int k = 0; k < 3; ++k)
        {
            bool value = rt[i][k];

            if(i == k)
                ASSERT_TRUE(value);
            else
                ASSERT_FALSE(value);
        }
    }
}

TEST_F(SIGRunRegionTransformTest, translation)
{
    RegionTransform rt;

    rt.update(glm::vec2(100, 100));

    for(int i = 0; i < 3; ++i)
    {
        for (int k = 0; k < 3; ++k)
        {
            bool value = rt[i][k];

            if(i == k)
                ASSERT_TRUE(value);
            else
            {
                if(k == 2 && (i == 0 || i == 1))
                    ASSERT_EQ(100, rt[i][k]);
                else
                    ASSERT_FALSE(value);
            }
        }
    }
}

TEST_F(SIGRunRegionTransformTest, rotation)
{
    RegionTransform rt;

    rt.update(glm::vec2(0, 0), 90);

    for(int i = 0; i < 3; ++i)
        for (int k = 0; k < 3; ++k)
            if(i == k && k != 2)
                ASSERT_TRUE(std::abs(rt[i][k]) < 0.0000001);
}

TEST_F(SIGRunRegionTransformTest, scale)
{
    RegionTransform rt;

    rt.update(glm::vec2(0, 0), 0, 2);

    for(int i = 0; i < 3; ++i)
        for (int k = 0; k < 3; ++k)
            if (i == k && k != 2)
                ASSERT_TRUE(rt[i][k] == 2.0);
}

TEST_F(SIGRunRegionTransformTest, transformation)
{
    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

    // translation
    RegionTransform rt;
    rt.update(glm::vec2(100, 0));

    for(auto& p: contour)
    {
        glm::vec3 q = p * rt.transform();
        q /= q.z;

        ASSERT_EQ(q.x, p.x + 100);
    }

    // scale
    rt = RegionTransform();
    rt.update(glm::vec2(0, 0), 0, 2);

    for(auto& p: contour)
    {
        glm::vec3 q = p * rt.transform();
        q /= q.z;

        ASSERT_EQ(q.x, p.x * 2);
        ASSERT_EQ(q.y, p.y * 2);
    }

    // rotation
    rt = RegionTransform();
    rt.update(glm::vec2(0, 0), 45, 1);

    std::transform(std::execution::par, contour.begin(), contour.end(), contour.begin(), [&rt](auto& p)
    {
        p = p * rt.transform();
        return p /= p.z;
    });

    ASSERT_NEAR(contour[0][0], 141.421345, 0.0001);
    ASSERT_NEAR(contour[0][1], 0, 0.0001);
    ASSERT_NEAR(contour[1][0], 494.974746, 0.0001);
    ASSERT_NEAR(contour[1][1], 353.553390, 0.0001);
    ASSERT_NEAR(contour[2][0], 848.528137, 0.0001);
    ASSERT_NEAR(contour[2][1], 0, 0.0001);
    ASSERT_NEAR(contour[3][0], 494.974746, 0.0001);
    ASSERT_NEAR(contour[3][1], -353.55339, 0.0001);
}

TEST_F(SIGRunRegionTransformTest, transformation_rotation_scale_center)
{
    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour_transformed(contour.size());

    glm::vec2 center(0, 0);

    std::transform(std::execution::par, contour.begin(), contour.end(), contour.begin(), [&center](const auto& p)
    {
        center.x += p.x;
        center.y += p.y;

        return p;
    });

    center.x /= contour.size();
    center.y /= contour.size();

    ASSERT_EQ(350, center.x);
    ASSERT_EQ(350, center.y);

    // rotation
    RegionTransform rt;
    rt.update(glm::vec2(0, 0), 45, 1);

    std::transform(std::execution::par, contour.begin(), contour.end(), contour_transformed.begin(), [&](const auto& p)
    {
        glm::vec3 q(p.x - center.x, p.y - center.y, 1);

        q = q * rt.transform();

        q /= q.z;

        q.x += center.x;
        q.y += center.y;

        return q;
    });

    ASSERT_NEAR(contour_transformed[0][0], -3.5533905, 0.0001);
    ASSERT_NEAR(contour_transformed[0][1], 350.0, 0.0001);
    ASSERT_NEAR(contour_transformed[1][0], 350.0, 0.0001);
    ASSERT_NEAR(contour_transformed[1][1], 703.553390, 0.0001);
    ASSERT_NEAR(contour_transformed[2][0], 703.553390, 0.0001);
    ASSERT_NEAR(contour_transformed[2][1], 350.0, 0.0001);
    ASSERT_NEAR(contour_transformed[3][0], 350.0, 0.0001);
    ASSERT_NEAR(contour_transformed[3][1], -3.5533905, 0.0001);

    rt = RegionTransform();
    rt.update(glm::vec2(0, 0), 0, 2);

    contour_transformed.clear();

    std::transform(std::execution::par, contour.begin(), contour.end(), contour_transformed.begin(), [&](const auto& p)
    {
        glm::vec3 q(p.x - center.x, p.y - center.y, 1);

        q = q * rt.transform();

        q /= q.z;

        q.x += center.x;
        q.y += center.y;

        return q;
    });

    ASSERT_NEAR(contour_transformed[0][0], -150.0, 0.0001);
    ASSERT_NEAR(contour_transformed[0][1], -150.0, 0.0001);
    ASSERT_NEAR(contour_transformed[1][0], -150.0, 0.0001);
    ASSERT_NEAR(contour_transformed[1][1], 850.0, 0.0001);
    ASSERT_NEAR(contour_transformed[2][0], 850.0, 0.0001);
    ASSERT_NEAR(contour_transformed[2][1], 850.0, 0.0001);
    ASSERT_NEAR(contour_transformed[3][0], 850.0, 0.0001);
    ASSERT_NEAR(contour_transformed[3][1], -150.0, 0.0001);
}
