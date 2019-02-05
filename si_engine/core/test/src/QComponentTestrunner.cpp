//
// Created by juergen on 05/02/19.
//

#include "../include/QComponentTestrunner.h"
#include "../include/cmd_args.h"

QComponentTestrunner::QComponentTestrunner() : QObject(), d_overall_result(0)
{}

void QComponentTestrunner::add_test(QObject *q_test)
{
    q_test->setParent(this);
    d_tests.append(q_test);
}

bool QComponentTestrunner::run_tests()
{
    QApplication qapp(e_argc, e_argv);

    QTimer::singleShot(0, this, SLOT(test()));

    qapp.exec();

    return d_overall_result == 0;
}

void QComponentTestrunner::test()
{
    run();

    QTest::qWait(1000);

    QApplication::instance()->quit();
}

void QComponentTestrunner::run()
{
    foreach(QObject *q_test, d_tests)
    {
        d_overall_result |= QTest::qExec(q_test);
    }
}
