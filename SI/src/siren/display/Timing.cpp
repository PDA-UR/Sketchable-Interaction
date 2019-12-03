
#include "Timing.hpp"
#include <SDL2/SDL.h>

FPSLimiter::FPSLimiter()
{

}

void FPSLimiter::initialize(float target_fps)
{
    set_target_fps(target_fps);
}

void FPSLimiter::set_target_fps(float target_fps)
{
    d_target_fps = target_fps;
}

void FPSLimiter::begin()
{
    d_start_ticks = SDL_GetTicks();
}

float FPSLimiter::end()
{
    calculate_fps();

    float frame_ticks = SDL_GetTicks() - d_start_ticks;

    // limit fps to max fps
    if (1000.0f / d_target_fps > frame_ticks)
        SDL_Delay(1000.0f / d_target_fps - frame_ticks);

    return d_fps;
}

void FPSLimiter::calculate_fps()
{
    static const int NUM_SAMPLES = 100;
    static float frame_times[NUM_SAMPLES];
    static int current_frame = 0;

    static float prev_ticks = SDL_GetTicks();
    float current_ticks;

    current_ticks = SDL_GetTicks();

    d_frame_time = current_ticks - prev_ticks;
    prev_ticks = current_ticks;

    frame_times[current_frame % NUM_SAMPLES] = d_frame_time;

    int count;
    current_frame++;

    if (current_frame < NUM_SAMPLES)
        count = current_frame;
    else
        count = NUM_SAMPLES;

    float average_frame_time = 0;

    for (int i = 0; i < count; i++)
        average_frame_time += frame_times[i];

    average_frame_time /= count;

    if (average_frame_time > 0)
        d_fps = 1000.0f / average_frame_time;
    else
        d_fps = 0.0f;
}