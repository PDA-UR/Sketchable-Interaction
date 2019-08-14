

#ifndef SI_SIMATHTEST_HPP
#define SI_SIMATHTEST_HPP

#include <gtest/gtest.h>

class SiMathTest : public ::testing::Test
{
public:
    virtual void SetUp()
    {

    }

    virtual void TearDown()
    {

    }
private:
protected:
};

class SiMathVector2PositiveIntersectionTest : public ::testing::TestWithParam<std::vector<std::vector<glm::vec2>>>
{
public:
    virtual void SetUp()
    {}

    virtual void TearDown()
    {}
};

class SiMathVector2NegativeIntersectionTest : public ::testing::TestWithParam<std::vector<std::vector<glm::vec2>>>
{
public:
    virtual void SetUp()
    {}

    virtual void TearDown()
    {}
};

#endif //SI_SIMATHTEST_HPP
