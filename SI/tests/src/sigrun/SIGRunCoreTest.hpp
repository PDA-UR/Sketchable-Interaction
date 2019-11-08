

#ifndef SITEST_SIGRUNCORETEST_HPP
#define SITEST_SIGRUNCORETEST_HPP

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <sigrun/Core.hpp>

class SIGRunCoreTest: public ::testing::Test
{
public:
    virtual void SetUp()
    {

    }

    virtual void TearDown()
    {

    }

    Core core;
};


class MockCore : public Core
{
public:
    MOCK_METHOD(void, start ,());
};


#endif //SITEST_SIGRUNCORETEST_HPP
