

#include "SIGRunTest.hpp"

using ::testing::AtLeast;

TEST(SIGRunTEST, construction)
{
    EXPECT_NO_FATAL_FAILURE(MockSIGRun mock_sigrun);
}

TEST(SIGRunTest, exec)
{
    int test_int = 0;
    char* test_string = "hello";
    SIGRun s;

    ASSERT_EQ(0, s.exec(test_int, &test_string));
}