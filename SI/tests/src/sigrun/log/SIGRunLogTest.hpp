

#ifndef SITEST_SIGRUNLOGTEST_HPP
#define SITEST_SIGRUNLOGTEST_HPP

#include <gtest/gtest.h>
#include <sigrun/MetaType.hpp>


class SIGRunLogTest: public ::testing::Test, public SIMetaType
{
public:
    virtual void SetUp()
    {
        META_TYPE("SIGRunLogTest")
    }

    virtual void TearDown()
    {

    }
};

class SIGRunLogLogLevelTest : public ::testing::TestWithParam<int>
{
public:
    virtual void SetUp()
    {}

    virtual void TearDown()
    {}
};

#endif //SITEST_SIGRUNLOGTEST_HPP
