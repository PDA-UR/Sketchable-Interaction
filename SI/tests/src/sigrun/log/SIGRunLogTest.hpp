

#ifndef SITEST_SIGRUNLOGTEST_HPP
#define SITEST_SIGRUNLOGTEST_HPP

#include <gtest/gtest.h>
#include <sigrun/SIObject.hpp>


class SIGRunLogTest: public ::testing::Test, public SIObject
{
public:
    virtual void SetUp()
    {SIOBJECT

    }

    virtual void TearDown()
    {

    }
};

#endif //SITEST_SIGRUNLOGTEST_HPP
