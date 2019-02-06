//
// Created by juergen on 01/02/19.
//

#include <QTest>
#include "gtest/gtest.h"

#include "../include/cmd_args.h"
#include "../include/si_engine_test.h"

int e_argc;
char **e_argv;

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    e_argc = argc;
    e_argv = argv;

    return RUN_ALL_TESTS();
}