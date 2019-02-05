//
// Created by juergen on 05/02/19.
//

#ifndef CORE_QCOMPONENTTESTRUNNER_H
#define CORE_QCOMPONENTTESTRUNNER_H

#include <QList>
#include <QTimer>
#include "../../include/si.h"
#include <QtTest>

class QComponentTestrunner : public QObject
{ Q_OBJECT

public:
    QComponentTestrunner();

    void add_test(QObject * q_test);
    bool run_tests();

private:
    void run();

    QList<QObject *> d_tests;
    int d_overall_result;

private slots:
    void test();
};


#endif //CORE_QCOMPONENTTESTRUNNER_H
