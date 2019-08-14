

#include <core/math/si_math.hpp>
#include "include/SiMathTest.hpp"


TEST_F(SiMathTest, Vector2f_construction)
{
    EXPECT_NO_FATAL_FAILURE(glm::vec2 v(1.0f, 0.0f));
    EXPECT_NO_FATAL_FAILURE(glm::vec2 u(0.0f, 0.1f));

    glm::vec2 v = glm::vec2(1.0f, 0.0f);

    EXPECT_NO_FATAL_FAILURE(glm::vec2 t(v));
}

TEST_F(SiMathTest, Vector2f_ymin)
{
    std::vector<std::vector<glm::vec2>> cases
    {
        std::vector<glm::vec2> {glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.1f)},

        // more cases
    };

    for(int i = 0; i < cases.size(); i++)
    {
        EXPECT_NO_FATAL_FAILURE(SI::SIMath::y_min(cases[i]));

        switch(i)
        {
            case 0: EXPECT_TRUE(0.0f == SI::SIMath::y_min(cases[i])); break;
        }
    }
}

TEST_F(SiMathTest, Vector2f_ymax)
{
    std::vector<std::vector<glm::vec2>> cases
    {
        std::vector<glm::vec2> {glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.1f)},

        // more cases
    };

    for(int i = 0; i < cases.size(); i++)
    {
        EXPECT_NO_FATAL_FAILURE(SI::SIMath::y_max(cases[i]));

        switch(i)
        {
            case 0: EXPECT_TRUE(1.0f == SI::SIMath::y_max(cases[i])); break;
        }
    }
}

TEST_F(SiMathTest, Vector2f_x)
{
    glm::vec2 v(0.0f, 0.0f);

    EXPECT_NO_FATAL_FAILURE(v.x);
    EXPECT_TRUE(v.x == 0.0f);
}

TEST_F(SiMathTest, Vector2f_y)
{
    glm::vec2 v(0.0f, 0.0f);

    EXPECT_NO_FATAL_FAILURE(v.y);
    EXPECT_TRUE(v.y == 0.0f);
}

TEST_F(SiMathTest, Vector2f_field_operator)
{
    glm::vec2 v(0.0f, 0.0f);

    EXPECT_NO_FATAL_FAILURE((v[0], v[1]));
    EXPECT_TRUE(0.0f == v[0]);
    EXPECT_TRUE(0.0f == v[1]);
}


std::vector<std::vector<std::vector<glm::vec2>>> positive_cases
{
    std::vector<std::vector<glm::vec2>>
    {
        std::vector<glm::vec2> { glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
        std::vector<glm::vec2> { glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 1.0f)}
    }

    // more samples
};

std::vector<std::vector<std::vector<glm::vec2>>> negative_cases
{
    std::vector<std::vector<glm::vec2>>
    {
        std::vector<glm::vec2> { glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
        std::vector<glm::vec2> { glm::vec2(0.0f, 2.0f), glm::vec2(1.0f, 2.0f)}
    }

    // more samples
};

INSTANTIATE_TEST_SUITE_P(Vector2_Intersection, SiMathVector2PositiveIntersectionTest, testing::ValuesIn(positive_cases));

TEST_P(SiMathVector2PositiveIntersectionTest, present_intersection)
{
    auto& v = GetParam()[0];
    auto& u = GetParam()[1];

    EXPECT_NO_FATAL_FAILURE(SI::SIMath::intersect(v, u));
    EXPECT_TRUE(SI::SIMath::intersect(v, u));
}

INSTANTIATE_TEST_SUITE_P(Vector2_Intersection, SiMathVector2NegativeIntersectionTest, testing::ValuesIn(negative_cases));

TEST_P(SiMathVector2NegativeIntersectionTest, no_intersection)
{
    auto& v = GetParam()[0];
    auto& u = GetParam()[1];

    EXPECT_NO_FATAL_FAILURE(SI::SIMath::intersect(v, u));
    EXPECT_FALSE(SI::SIMath::intersect(v, u));
}