

#ifndef SI_SIENGINETEST_HPP
#define SI_SIENGINETEST_HPP

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "SIEngine.hpp"

class SIEngineTest : public ::testing::Test
{
public:
    virtual void SetUp()
    {

    }

    virtual void TearDown()
    {

    }
};

#endif //SI_SIENGINETEST_HPP
