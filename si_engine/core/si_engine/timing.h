//
// Created by juergen on 22/01/19.
//

#ifndef CORE_TIMING_H
#define CORE_TIMING_H

#include <chrono>

namespace si
{
    namespace // necessary for anonymous namespace, so static class can be accessed locally in each file
    {

        class time
        {
        public:
            static double get_time()
            {
                return std::chrono::duration_cast<std::chrono::nanoseconds>(
                        std::chrono::system_clock::now() - s_time_point).count() / 1000000000.0;
            }

            static void set_time_delta(const double td)
            {
                time::s_time_delta = td;
            }

            static double time_delta()
            {
                return s_time_delta;
            }

        private:
            static double s_time_delta;
            static std::chrono::system_clock::time_point s_time_point;
        };

        double time::s_time_delta = 0.0;
        std::chrono::system_clock::time_point time::s_time_point = std::chrono::system_clock::now();
    }
}

#endif //CORE_TIMING_H
