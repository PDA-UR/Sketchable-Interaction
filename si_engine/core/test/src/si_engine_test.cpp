//
// Created by juergen on 01/02/19.
//

#include <QTest>
#include <QSignalSpy>
#include "../include/si_engine_test.h"
#include "../include/cmd_args.h"
#include "../../include/si.h"
#include "../../include/engine.h"
#include "../../include/step.h"

TEST_F(SIEngineTest, lazy_init)
{
    si::Engine *e = si::Engine::__instance__();
    e->clear_active_regions();

    si::SI *si = new si::SI(e_argc, e_argv);

    EXPECT_NE(e, nullptr);

    si->quit();

    delete si;
    si = nullptr;
}

TEST_F(SIEngineTest, singleton)
{
    si::Engine *e = si::Engine::__instance__();
    e->clear_active_regions();

    si::SI *si = new si::SI(e_argc, e_argv);

    si::Engine *e1 = si::Engine::__instance__();
    si::Engine *e2 = si::Engine::__instance__();

    EXPECT_EQ(e1, e2);

    si->quit();

    delete si;
    si = nullptr;
}

TEST_F(SIEngineTest, is_running)
{
    si::Engine *e = si::Engine::__instance__();
    e->clear_active_regions();

    si::SI *si = new si::SI(e_argc, e_argv);

    EXPECT_TRUE(e->is_running());

    e->stop();

    EXPECT_FALSE(e->is_running());

    si->quit();

    delete si;
    si = nullptr;
}

TEST_F(SIEngineTest, active_regions)
{
    si::Engine *e = si::Engine::__instance__();

    si::SI *si = new si::SI(e_argc, e_argv);

    EXPECT_TRUE(e->has_active_regions());

    e->clear_active_regions();

    EXPECT_FALSE(e->has_active_regions());

    si->quit();

    delete si;
    si = nullptr;
}

TEST_F(SIEngineTest, main_canvas_reference)
{
    si::Engine *e = si::Engine::__instance__();
    e->clear_active_regions();

    si::SI *si = new si::SI(e_argc, e_argv);

    EXPECT_NE(e->i_main_canvas_region(), nullptr);

    si->quit();

    delete si;
    si = nullptr;
}

TEST_F(SIEngineTest, step_reference)
{
    si::Engine *e = si::Engine::__instance__();
    e->clear_active_regions();

    si::SI *si = new si::SI(e_argc, e_argv);

    EXPECT_NE(si::Engine::__instance__()->i_step(), nullptr);

    si->quit();

    delete si;
    si = nullptr;
}

TEST_F(SIEngineTest, add_region_template)
{
    si::Engine *e = si::Engine::__instance__();
    e->clear_active_regions();

    si::SI *si = new si::SI(e_argc, e_argv);

    EXPECT_ANY_THROW(si::Engine::__instance__()->add_region_template(nullptr));
    EXPECT_NO_THROW(si::Engine::__instance__()->add_region_template(new si::region));

    si->quit();

    delete si;
    si = nullptr;
}

// for some reson this has to be last one called as of now probably due to QSignalSpy
TEST_F(SIEngineTest, step_tick)
{
    si::Engine *e = si::Engine::__instance__();
    e->clear_active_regions();

    si::SI *si = new si::SI(e_argc, e_argv);

    QSignalSpy spy(si::Engine::__instance__()->i_step().get(), &si::step::on_step);

    si::Engine::__instance__()->i_step().get()->on_step();

    EXPECT_TRUE(spy.count());
    EXPECT_TRUE(spy.isValid());

    si->quit();

    delete si;
    si = nullptr;
}