

#include <core/math/si_math.hpp>
#include "include/SiMathTest.hpp"


TEST_F(SiMathTest, Vector2f_construction)
{
    EXPECT_NO_FATAL_FAILURE(Vector2<float> v(1.0f, 0.0f));
    EXPECT_NO_FATAL_FAILURE(Vector2<float> u(0.0f, 0.1f));

    Vector2<float> v = Vector2<float>(1.0f, 0.0f);

    EXPECT_NO_FATAL_FAILURE(Vector2<float> t(v));
}

TEST_F(SiMathTest, Vector2f_ymin)
{
    std::vector<std::vector<Vector2<float>>> cases
    {
        std::vector<Vector2<float>> {Vector2<float>(0.0f, 1.0f), Vector2<float>(0.0f, 0.0f), Vector2<float>(0.0f, 0.1f)},

        // more cases
    };

    for(int i = 0; i < cases.size(); i++)
    {
        EXPECT_NO_FATAL_FAILURE(Vector2<float>::y_min(cases[i]));

        switch(i)
        {
            case 0: EXPECT_TRUE(0.0f == Vector2<float>::y_min(cases[i])); break;
        }
    }
}

TEST_F(SiMathTest, Vector2f_ymax)
{
    std::vector<std::vector<Vector2<float>>> cases
    {
        std::vector<Vector2<float>> {Vector2<float>(0.0f, 1.0f), Vector2<float>(0.0f, 0.0f), Vector2<float>(0.0f, 0.1f)},

        // more cases
    };

    for(int i = 0; i < cases.size(); i++)
    {
        EXPECT_NO_FATAL_FAILURE(Vector2<float>::y_max(cases[i]));

        switch(i)
        {
            case 0: EXPECT_TRUE(1.0f == Vector2<float>::y_max(cases[i])); break;
        }
    }
}

TEST_F(SiMathTest, Vector2f_x)
{
    Vector2<float> v(0.0f, 0.0f);

    EXPECT_NO_FATAL_FAILURE(v.x());
    EXPECT_TRUE(v.x() == 0.0f);
}

TEST_F(SiMathTest, Vector2f_y)
{
    Vector2<float> v(0.0f, 0.0f);

    EXPECT_NO_FATAL_FAILURE(v.y());
    EXPECT_TRUE(v.y() == 0.0f);
}

TEST_F(SiMathTest, Vector2f_field_operator)
{
    Vector2<float> v(0.0f, 0.0f);

    EXPECT_NO_FATAL_FAILURE((v[0], v[1]));
    EXPECT_TRUE(0.0f == v[0]);
    EXPECT_TRUE(0.0f == v[1]);
}


std::vector<std::vector<std::vector<Vector2<float>>>> positive_cases
{
    std::vector<std::vector<Vector2<float>>>
    {
        std::vector<Vector2<float>> { Vector2<float>(0.0f, 0.0f), Vector2<float>(1.0f, 0.0f)},
        std::vector<Vector2<float>> { Vector2<float>(0.0f, 0.0f), Vector2<float>(0.0f, 1.0f)}
    }

    // more samples
};

std::vector<std::vector<std::vector<Vector2<float>>>> negative_cases
{
    std::vector<std::vector<Vector2<float>>>
    {
        std::vector<Vector2<float>> { Vector2<float>(0.0f, 0.0f), Vector2<float>(1.0f, 0.0f)},
        std::vector<Vector2<float>> { Vector2<float>(0.0f, 2.0f), Vector2<float>(1.0f, 2.0f)}
    }

    // more samples
};

INSTANTIATE_TEST_SUITE_P(Vector2_Intersection, SiMathVector2PositiveIntersectionTest, testing::ValuesIn(positive_cases));

TEST_P(SiMathVector2PositiveIntersectionTest, present_intersection)
{
    auto& v = GetParam()[0];
    auto& u = GetParam()[1];

    EXPECT_NO_FATAL_FAILURE(Vector2<float>::intersect(v, u));
    EXPECT_TRUE(Vector2<float>::intersect(v, u));
}

INSTANTIATE_TEST_SUITE_P(Vector2_Intersection, SiMathVector2NegativeIntersectionTest, testing::ValuesIn(negative_cases));

TEST_P(SiMathVector2NegativeIntersectionTest, no_intersection)
{
    auto& v = GetParam()[0];
    auto& u = GetParam()[1];

    EXPECT_NO_FATAL_FAILURE(Vector2<float>::intersect(v, u));
    EXPECT_FALSE(Vector2<float>::intersect(v, u));
}