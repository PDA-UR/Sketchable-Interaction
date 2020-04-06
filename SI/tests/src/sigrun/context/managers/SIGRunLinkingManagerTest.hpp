

#ifndef SITEST_SIGRUNLINKINGMANAGERTEST_HPP
#define SITEST_SIGRUNLINKINGMANAGERTEST_HPP

#include <sigrun/context/Context.hpp>
#include <gtest/gtest.h>
#include <sigrun/SIObject.hpp>

class SIGRunLinkingManagerTest: public ::testing::Test, public SIObject
{ SITEST
public:
    virtual void SetUp()
    {

    }

    virtual void TearDown()
    {

    }

    Context d_c;
};


#endif //SITEST_SIGRUNLINKINGMANAGERTEST_HPP
