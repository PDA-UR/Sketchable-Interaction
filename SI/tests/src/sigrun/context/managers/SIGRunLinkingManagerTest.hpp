

#ifndef SITEST_SIGRUNLINKINGMANAGERTEST_HPP
#define SITEST_SIGRUNLINKINGMANAGERTEST_HPP

#include <gtest/gtest.h>
#include <sigrun/SIObject.hpp>

class SIGRunLinkingManagerTest: public ::testing::Test, public SIObject
{
public:
    virtual void SetUp()
    {SIOBJECT(SITEST)

    }

    virtual void TearDown()
    {

    }
};


#endif //SITEST_SIGRUNLINKINGMANAGERTEST_HPP
