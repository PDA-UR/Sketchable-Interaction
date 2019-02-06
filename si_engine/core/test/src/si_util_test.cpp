//
// Created by juergen on 06/02/19.
//

#include "../include/si_util_test.h"

#include "../../include/util.h"

TEST_F(SIUtilTest, vector_contains_item)
{
    std::vector<int> i_test {
        2, 3, 4, 5, 6, 7
    };

    EXPECT_TRUE(si::contains(i_test, 7) > -1);
    EXPECT_FALSE(si::contains(i_test, 8) > -1);
}