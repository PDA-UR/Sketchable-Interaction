//
// Created by juergen on 25/01/19.
//

#ifndef CORE_STEP_H
#define CORE_STEP_H


#include <QThread>

namespace si
{
class step : public QThread
{Q_OBJECT

public:
    explicit step(double frequency, bool _sleep=true);

    void run() override;
    void stop();

private:
    bool d_is_running = false;
    double d_frequency = 0.0;
    bool d_sleep = true;

    signals:
        void on_step();
};

}


#endif //CORE_STEP_H
