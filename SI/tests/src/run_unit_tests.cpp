#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <sigrun/log/Log.hpp>

#define NUM_TEST_ITERATIONS 1

int main(int argc, char** argv)
{
    Log::SHOW = LOG_SHOW_ALL;

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::InitGoogleMock(&argc, argv);

    int ret = 0;

    for(int i = 0; i < NUM_TEST_ITERATIONS; ++i)
        ret &= RUN_ALL_TESTS();

    return ret;
}
