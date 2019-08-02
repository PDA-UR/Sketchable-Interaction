

#ifndef SI_SISCRIPTINGTEST_HPP
#define SI_SISCRIPTINGTEST_HPP

#include <core/pysi_core/Scripting.hpp>
#include <gtest/gtest.h>

class SIScriptingTest: public ::testing::Test
{
public:
    virtual void SetUp()
    {
        d_script = Scripting();
    }

    virtual void TearDown()
    {

    }

    Scripting d_script;
};


#endif //SI_SISCRIPTINGTEST_HPP
