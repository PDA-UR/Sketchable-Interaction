//
// Created by juergen on 06/02/19.
//

#include "../include/si_util_test.h"
#include "../../include/util.h"

std::vector<int> ei_test
{
    2, 3, 4, 5, 6, 7
};

INSTANTIATE_TEST_SUITE_P(ContainsInt, SIUtilTestInt, testing::ValuesIn(ei_test));

TEST_P(SIUtilTestInt, vector_contains_int_item_true)
{
    std::vector<int> i_test
    {
        2, 3, 4, 5, 6, 7
    };

    EXPECT_TRUE(si::contains(i_test, GetParam()) > -1);
}

TEST_P(SIUtilTestInt, vector_contains_int_item_false)
{
    std::vector<int> i_test
    {
        1, 8, 0, -5, -1, 10
    };

    EXPECT_TRUE(si::contains(i_test, GetParam()) == -1);
}


std::vector<std::string> es_test
{
    "si", "test", "true", "or", "false", "oachkatzlschwoaf"
};

INSTANTIATE_TEST_SUITE_P(ContainsString, SIUtilTestString, testing::ValuesIn(es_test));

TEST_P(SIUtilTestString, vector_contains_string_item_true)
{
    std::vector<std::string> i_test
    {
        "si", "test", "true", "or", "false", "oachkatzlschwoaf"
    };

    EXPECT_TRUE(si::contains(i_test, GetParam()) > -1);
}

TEST_P(SIUtilTestString, vector_contains_string_item_false)
{
    std::vector<std::string> i_test
    {
        "no si", "no test", "not true", "not false", "no eichhörnchenschwanz"
    };

    EXPECT_TRUE(si::contains(i_test, GetParam()) == -1);
}