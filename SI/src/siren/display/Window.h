

#ifndef SITEST_WINDOW_H
#define SITEST_WINDOW_H

#include <SDL2/SDL.h>

#ifdef __cplusplus
extern "C" {
#endif

enum _WindowFlags_
{
    INVISIBLE = 0x1,
    FULLSCREEN = 0x2,
    BORDERLESS = 0x4
};

struct _Window_
{
    SDL_Window* window;
    SDL_GLContext gl_context;
    int width;
    int height;
};

typedef enum _WindowFlags_ WindowFlags;
typedef struct _Window_ Window;

int window_create(Window* w, const char* name, int width, int height, unsigned int flags);

void window_swap_buffer(Window* w);

#ifdef __cplusplus
}
#endif


#endif //SITEST_WINDOW_H
