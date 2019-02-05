//
// Created by juergen on 01/02/19.
//

#include "../../include/si.h"
#include "gtest/gtest.h"
#include "../include/si_test.h"
#include "../include/cmd_args.h"
#include "../../include/engine.h"

TEST_F(SITest, cpp_constructor)
{
    si::SI *si = new si::SI(e_argc, e_argv);

    EXPECT_NE(si, nullptr);

    QApplication::instance()->quit();

    delete si;
    si = nullptr;
}

TEST_F(SITest, c_instance_creation)
{
    void *si = si::si_create_instance(e_argc, e_argv);

    EXPECT_NE(si, nullptr);

    si::SI *_si = reinterpret_cast<si::SI *>(si);

    EXPECT_NE(_si, nullptr);
    EXPECT_NO_THROW(si::si_create_instance(e_argc, e_argv));

    QApplication::instance()->quit();

    si = nullptr;

    delete _si;
    _si = nullptr;
}

TEST_F(SITest, c_delete_instance)
{
    void *si = si::si_create_instance(e_argc, e_argv);

    EXPECT_NO_THROW(si::si_delete_instance(si));

    QApplication::instance()->quit();
}

TEST_F(SITest, cpp_add_region)
{
    si::SI *si = new si::SI(e_argc, e_argv);

    EXPECT_ANY_THROW(si->add_region(nullptr));
    EXPECT_NO_THROW(si->add_region((void *) new si::region()));

    QApplication::instance()->quit();

    delete si;
    si = nullptr;
}

TEST_F(SITest, c_add_region)
{
    void *si = si::si_create_instance(e_argc, e_argv);

    EXPECT_ANY_THROW(si::si_add_region(si, nullptr));
    EXPECT_NO_THROW(si::si_add_region(si, (void *) new si::region()));

    QApplication::instance()->quit();
}

TEST_F(SITest, c_create_region_instance)
{
    si::SI *si = reinterpret_cast<si::SI *>(si::si_create_instance(e_argc, e_argv));

    si::region_callback rce = callback_test;
    si::region_callback rcc = callback_test;
    si::region_callback rcl = callback_test;

    EXPECT_NO_THROW(si::si_region_create_instance(rce, rcc, rcl));

    rce = nullptr;

    EXPECT_ANY_THROW(si::si_region_create_instance(rce, rcc, rcl));

    rce = callback_test;
    rcc = nullptr;

    EXPECT_ANY_THROW(si::si_region_create_instance(rce, rcc, rcl));

    rcc = callback_test;
    rcl = nullptr;

    EXPECT_ANY_THROW(si::si_region_create_instance(rce, rcc, rcl));

    QApplication::instance()->quit();

    delete si;
    si = nullptr;
}