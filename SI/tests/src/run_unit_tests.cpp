#include <gmock/gmock.h>
#include <gtest/gtest.h>

#define NUM_TEST_ITERATIONS 250

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::InitGoogleMock(&argc, argv);

    for(int i = 0; i < NUM_TEST_ITERATIONS - 1; ++i)
        RUN_ALL_TESTS();

    return RUN_ALL_TESTS();
}
