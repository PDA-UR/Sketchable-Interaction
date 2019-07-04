

#include "SI_test.hpp"

TEST_F(SITest, initialize)
{
    ASSERT_EQ(SI_initialize(""), 2);
    ASSERT_EQ(SI_initialize(nullptr), 1);
    ASSERT_EQ(SI_initialize("test"), 0);
}

TEST_F(SITest, start)
{
    ASSERT_EQ(SI_start(), 0);
}

TEST_F(SITest, pause)
{
    ASSERT_EQ(SI_pause(), 0);
}

TEST_F(SITest, end)
{
    ASSERT_EQ(SI_end(), 0);
}
