

#ifndef SITEST_TIMING_HPP
#define SITEST_TIMING_HPP


class FPSLimiter
{
public:
    FPSLimiter();

    void initialize(float target_fps);
    void set_target_fps(float target_fps);
    void begin();
    float end(); // returns fps
private:
    void calculate_fps();

    unsigned int d_start_ticks;
    float d_target_fps;
    float d_frame_time, d_fps;
};


#endif //SITEST_TIMING_HPP
