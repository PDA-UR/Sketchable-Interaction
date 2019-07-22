

#include "include/SIRuntime_test.hpp"

TEST_F(SIRuntimeTest, initialize)
{
    ASSERT_EQ(SI_initialize(""), 2);
    ASSERT_EQ(SI_initialize(nullptr), 1);
    ASSERT_EQ(SI_initialize("tests"), 0);
}

TEST_F(SIRuntimeTest, start)
{
    ASSERT_EQ(SI_start(), 0);
}

TEST_F(SIRuntimeTest, pause)
{
    ASSERT_EQ(SI_pause(), 0);
}

TEST_F(SIRuntimeTest, end)
{
    ASSERT_EQ(SI_end(), 0);
}
