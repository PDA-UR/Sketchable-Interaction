//
// Created by juergen on 01/02/19.
//

#include <QTest>
#include "gtest/gtest.h"

#include "../include/cmd_args.h"
#include "../include/si_engine_test.h"

#include "../include/QComponentTestrunner.h"

int e_argc;
char **e_argv;

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    e_argc = argc;
    e_argv = argv;

    //QComponentTestrunner qctr;
    //qctr.add_test(new QComponentTest);

    //qDebug() << "Overall result: " << (qctr.run_tests() ? "PASS":"FAIL");

    return RUN_ALL_TESTS();
}