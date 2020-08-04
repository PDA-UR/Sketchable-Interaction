# include "Timing.hpp"

double Time::s_time_delta = 0.0;
std::chrono::system_clock::time_point Time::m_epoch = std::chrono::system_clock::now();


