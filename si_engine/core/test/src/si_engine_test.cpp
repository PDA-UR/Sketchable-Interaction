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
#include "../include/QComponentTestrunner.h"

void QComponentTest::initTestCase()
{

}

void QComponentTest::toUpper()
{
    QString str = "Hello";
    QCOMPARE(str.toUpper() == "HELLO", true);
}

void QComponentTest::cleanupTestCase()
{

}

void QComponentTest::init()
{

}

void QComponentTest::cleanup()
{

}

TEST_F(SIEngineTest, lazy_init)
{
    si::Engine *e = si::Engine::__instance__();

    EXPECT_NE(e, nullptr);
}

TEST_F(SIEngineTest, singleton)
{
    si::Engine *e1 = si::Engine::__instance__();
    si::Engine *e2 = si::Engine::__instance__();

    EXPECT_EQ(e1, e2);
}

TEST_F(SIEngineTest, step_reference)
{
    EXPECT_NE(si::Engine::__instance__()->i_step(), nullptr);
}

TEST_F(SIEngineTest, step_tick)
{
    si::Engine::__instance__()->start();

    auto asyncFuture = std::async(std::launch::async, [&]()
    {
        QSignalSpy spy(si::Engine::__instance__()->i_step().get(), &si::step::on_step);

        EXPECT_TRUE(spy.wait(1000));
        si::Engine::__instance__()->stop();
    });

    EXPECT_TRUE(asyncFuture.wait_for(std::chrono::milliseconds(1000)) != std::future_status::timeout);
}

TEST_F(SIEngineTest, blah)
{
    si::Engine::__instance__()->start();

}