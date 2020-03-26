

#ifndef SITEST_SIGRUNLOGTEST_HPP
#define SITEST_SIGRUNLOGTEST_HPP

#include <gtest/gtest.h>
#include <sigrun/SIObject.hpp>


class SIGRunLogTest: public ::testing::Test, public SIObject
{ SITEST
public:
    virtual void SetUp()
    {

    }

    virtual void TearDown()
    {

    }
};

#endif //SITEST_SIGRUNLOGTEST_HPP
