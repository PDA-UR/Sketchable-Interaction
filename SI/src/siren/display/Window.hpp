

#ifndef SITEST_WINDOW_HPP
#define SITEST_WINDOW_HPP

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include <string>

enum WindowFlags
{
    INVISIBLE = 0x1,
    FULLSCREEN = 0x2,
    BORDERLESS = 0x4
};

class Window
{
public:
    Window();

    ~Window();

    int create(const std::string &window_name, int width, int height, unsigned int flags);

    void swap_buffer();

    int width() const;

    void set_width(int width);

    int height() const;

    void set_height(int height);

private:
    SDL_Window *p_window;
    SDL_GLContext gl_context;

    int d_width, d_height;
};

#endif //SITEST_WINDOW_HPP