//
// Created by juergen on 25/01/19.
//

#include "../include/step.h"

namespace si
{
    step::step(double frequency, bool _sleep) :
            d_is_running(true),
            d_frequency(frequency),
            d_sleep(_sleep)
    {}

    void step::run()
    {
        while (d_is_running)
        {
            if (d_sleep)
                usleep(16667);

            emit on_step();
        }
    }

    void step::stop()
    {
        d_is_running = false;
    }
}
