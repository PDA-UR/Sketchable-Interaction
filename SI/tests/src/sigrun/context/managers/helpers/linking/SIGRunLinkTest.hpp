

#ifndef SITEST_SIGRUNLINKTEST_HPP
#define SITEST_SIGRUNLINKTEST_HPP

#include <sigrun/context/Context.hpp>
#include <gtest/gtest.h>

class SIGRunLinkTest: public ::testing::Test
{
public:
    virtual void SetUp()
    {

    }

    virtual void TearDown()
    {

    }

    Context d_c;
};


#endif //SITEST_SIGRUNLINKTEST_HPP
