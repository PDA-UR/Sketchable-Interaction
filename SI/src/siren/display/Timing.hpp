

#ifndef SITEST_TIMING_HPP
#define SITEST_TIMING_HPP

#include <chrono>

class Time
{
public:
    static double get_time()
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::system_clock::now() -
                m_epoch).count()  / 1000000000.0;
    }

    static void set_time_delta(double td)
    {
        Time::s_time_delta = td;
    }

    static double time_delta() { return s_time_delta; }

private:
    static double s_time_delta;
    static std::chrono::system_clock::time_point m_epoch;
};

double Time::s_time_delta = 0.0;
std::chrono::system_clock::time_point Time::m_epoch = std::chrono::system_clock::now();

#endif //SITEST_TIMING_HPP
