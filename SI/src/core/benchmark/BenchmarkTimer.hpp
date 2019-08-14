

#ifndef SITEST_BENCHMARKTIMER_HPP
#define SITEST_BENCHMARKTIMER_HPP

#include <chrono>

#define SI_BENCHMARK(...) { SI::BenchmarkTimer timer; __VA_ARGS__ }

namespace SI
{
    class BenchmarkTimer
    {
    public:
        BenchmarkTimer();
        ~BenchmarkTimer();

    private:
        void stop();
        std::chrono::time_point<std::chrono::high_resolution_clock> d_start_point;
    };
}

#endif //SITEST_BENCHMARKTIMER_HPP
