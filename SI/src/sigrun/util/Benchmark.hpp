

#ifndef SITEST_BENCHMARK_HPP
#define SITEST_BENCHMARK_HPP

#include <chrono>
#include "../log/Log.hpp"

#define SI_BENCHMARK(...) { BenchmarkTimer timer; __VA_ARGS__ }

class BenchmarkTimer: public SIObject
{ SIGRUN

public:
    BenchmarkTimer():
        d_start_point(std::chrono::high_resolution_clock::now())
    {
    }

    ~BenchmarkTimer()
    {
        stop();
    }

private:
    void stop()
    {
        auto start = std::chrono::time_point_cast<std::chrono::microseconds>(d_start_point).time_since_epoch().count();
        auto end = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count();
        auto duration = end - start;

        INFO("Duration: " + std::to_string(duration * 0.001));
    }

    std::chrono::time_point<std::chrono::high_resolution_clock> d_start_point;
};

#endif //SITEST_BENCHMARK_HPP
