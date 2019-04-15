//
// Created by juergen on 11/02/19.
//

#include "../include/si_debug_test.h"


TEST_F(SIDebugTest, print)
{
    EXPECT_NO_FATAL_FAILURE(si::debug::print("blah", 1, 2.4f, std::string("hello")));
    EXPECT_NO_THROW(si::debug::print("blah", 1, 2.4f, std::string("hello")));
}

// in the future add representations for si objects to be printed via debug
// in the future look into iteratively printing lists