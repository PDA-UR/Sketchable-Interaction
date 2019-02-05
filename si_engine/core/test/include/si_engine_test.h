//
// Created by juergen on 01/02/19.
//

#ifndef CORE_SI_ENGINE_TEST_H
#define CORE_SI_ENGINE_TEST_H

#include "gtest/gtest.h"
#include "../../include/si.h"
#include "../../include/debug.h"
#include "cmd_args.h"

class SIEngineTest : public ::testing::Test
{
public:
    virtual void SetUp()
    {
        si = new si::SI(e_argc, e_argv);
    }

    virtual void TearDown()
    {
        delete si;
        si = nullptr;
    }

    bool d_is_step_ticking = false;

private:
    si::SI *si = nullptr;

};

class step_tick_test : public QObject
{ Q_OBJECT

public:
    step_tick_test() = default;

    bool is_ticking()
    {
        return d_is_step_ticking;
    }

private:
    bool d_is_step_ticking = false;

public slots:
    void on_step_tick_test()
    {
        d_is_step_ticking = true;
        si::debug::print("WTF");
    };
};

class QComponentTest: public QObject
{ Q_OBJECT

public:
    QComponentTest() : QObject()
    {}

private:

private slots:
    void initTestCase();
    void toUpper();
    void cleanupTestCase();
    void init();
    void cleanup();
};

#endif //CORE_SI_ENGINE_TEST_H
