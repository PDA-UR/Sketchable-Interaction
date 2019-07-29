

#include "include/SIRuntimeTest.hpp"


TEST_F(SIRuntimeTest, start)
{
    //ASSERT_EQ(SI_start("plugins"), 0);
}

TEST_F(SIRuntimeTest, pause)
{
    ASSERT_EQ(SI_pause(), 0);
}

TEST_F(SIRuntimeTest, end)
{
//    ASSERT_EQ(SI_end(), 0);
}
