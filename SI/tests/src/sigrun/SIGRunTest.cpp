

#include "SIGRunTest.hpp"

TEST(SIGRunTEST, construction)
{
    EXPECT_NO_FATAL_FAILURE(MockSIGRun mock_sigrun);
}

TEST(SIGRunTest, exec)
{
    int test_int = 0;
    char* test_string = "hello";
    MockSIGRun s;

    ASSERT_EQ(0, s.exec(test_int, &test_string));
}

TEST(SIGRunTest, quit)
{
    ASSERT_EQ(MockSIGRun::quit(), 0);
}