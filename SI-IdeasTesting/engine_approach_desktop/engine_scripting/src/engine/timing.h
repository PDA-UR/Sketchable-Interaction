
#ifndef ENGINE_SCRIPTING_TIMING_H
#define ENGINE_SCRIPTING_TIMING_H

#include <chrono>

class time
{
public:
    static double get_time()
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::system_clock::now() - time_point).count() / 1000000000.0;
    }

    static void set_time_delta(const double td)
    {
        time::time_delta = td;
    }

private:
    static double time_delta;
    static std::chrono::system_clock::time_point time_point;
};

double time::time_delta = 0.0;
std::chrono::system_clock::time_point time::time_point = std::chrono::system_clock::now();

#endif //ENGINE_SCRIPTING_TIMING_H
