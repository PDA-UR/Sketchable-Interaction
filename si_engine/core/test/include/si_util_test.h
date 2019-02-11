//
// Created by juergen on 06/02/19.
//

#ifndef CORE_SI_UTIL_TEST_H
#define CORE_SI_UTIL_TEST_H


#include <gtest/gtest.h>

class SIUtilTestInt : public ::testing::TestWithParam<int>
{
public:
    virtual void SetUp()
    {}

    virtual void TearDown()
    {}
};

class SIUtilTestString : public ::testing::TestWithParam<std::string>
{
public:
    virtual void SetUp()
    {}

    virtual void TearDown()
    {}
};


#endif //CORE_SI_UTIL_TEST_H
