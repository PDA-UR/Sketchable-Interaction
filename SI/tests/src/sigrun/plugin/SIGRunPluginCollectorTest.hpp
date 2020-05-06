
#ifndef SITEST_SIGRUNPLUGINCOLLECTORTEST_HPP
#define SITEST_SIGRUNPLUGINCOLLECTORTEST_HPP

#include <gmock/gmock.h>
#include <gtest/gtest.h>

class SIGRunPluginCollectorTest: public ::testing::Test
{
public:
    virtual void SetUp()
    {

    }

    virtual void TearDown()
    {

    }

    const std::string PATH = "tests/res/plugin_collector";

};


#endif //SITEST_SIGRUNPLUGINCOLLECTORTEST_HPP
