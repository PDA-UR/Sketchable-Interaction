#include <gmock/gmock.h>
#include <gtest/gtest.h>

#define NUM_TEST_ITERATIONS 1

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::InitGoogleMock(&argc, argv);

    int ret = 0;

    for(int i = 0; i < NUM_TEST_ITERATIONS; ++i)
        ret &= RUN_ALL_TESTS();

    return ret;
}
