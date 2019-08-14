

#include <iostream>
#include "BenchmarkTimer.hpp"

SI::BenchmarkTimer::BenchmarkTimer() :
    d_start_point(std::chrono::high_resolution_clock::now())
{
}

SI::BenchmarkTimer::~BenchmarkTimer()
{
    stop();
}

void SI::BenchmarkTimer::stop()
{
    auto start = std::chrono::time_point_cast<std::chrono::microseconds>(d_start_point).time_since_epoch().count();
    auto end = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count();
    auto duration = end - start;

    double ms = duration * 0.001;

    std::cout << duration << "us" << "(" << ms << "ms)" << std::endl;

    // needs tracker and output to file, etc
}
