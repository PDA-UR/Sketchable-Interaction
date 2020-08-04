

#ifndef SITEST_BENCHMARK_HPP
#define SITEST_BENCHMARK_HPP

#include <chrono>
#include "../log/Log.hpp"
#include <sigrun/SITypes.hpp>

#define SI_BENCHMARK_SCOPE(...) { BenchmarkTimer timer; __VA_ARGS__ }
#define SI_BENCHMARK_START NonScopedBenchmarkTimer::instance().mark_start()
#define SI_BENCHMARK_STOP NonScopedBenchmarkTimer::instance().mark_stop()

class NonScopedBenchmarkTimer: public SIObject
{ SIGRUN
public:
    static NonScopedBenchmarkTimer& instance()
    {
        static NonScopedBenchmarkTimer instance;

        return instance;
    }

    NonScopedBenchmarkTimer(NonScopedBenchmarkTimer const&) = delete;
    void operator=(NonScopedBenchmarkTimer const&) = delete;

    void mark_start()
    {
        d_start_point = std::chrono::high_resolution_clock::now();
    }

    void mark_stop()
    {
        auto end = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count();
        auto start = std::chrono::time_point_cast<std::chrono::microseconds>(d_start_point).time_since_epoch().count();
        auto duration = end - start;

        WARN("Duration: " + std::to_string(duration * 0.001) + " ms");

        exit(0);
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> d_start_point;

    NonScopedBenchmarkTimer() = default;
};

class ScopedBenchmarkTimer: public SIObject
{ SIGRUN

public:
    ScopedBenchmarkTimer():
        d_start_point(std::chrono::high_resolution_clock::now())
    {
    }

    ~ScopedBenchmarkTimer()
    {
        stop();
    }

private:
    void stop()
    {
        auto end = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count();
        auto start = std::chrono::time_point_cast<std::chrono::microseconds>(d_start_point).time_since_epoch().count();
        auto duration = end - start;

        WARN("Duration: " + std::to_string(duration * 0.001) + " ms");
    }

    std::chrono::time_point<std::chrono::high_resolution_clock> d_start_point;
};

#endif //SITEST_BENCHMARK_HPP
