

#include <render_engine/debug/SiRenPrint.hpp>
#include "Window.hpp"

namespace SIRen
{
    Window::Window()
    {}

    Window::~Window()
    {

    }

    int Window::create(const std::string &window_name, int width, int height, unsigned int flags)
    {
        uint32_t window_flags = SDL_WINDOW_OPENGL;

        if (flags & INVISIBLE)
            window_flags |= SDL_WINDOW_HIDDEN;

        if (flags & FULLSCREEN)
            window_flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

        if (flags & BORDERLESS)
            window_flags |= SDL_WINDOW_BORDERLESS;

        p_window = SDL_CreateWindow(window_name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, d_width,
                                    d_height, window_flags);
        gl_context = SDL_GL_CreateContext(p_window);

        glewInit();

        SIRenPrint::print("*** OPENGL VERSION: " + std::string((const char *) glGetString(GL_VERSION)) + " ***");

        glClearColor(0, 0, 1, 1);

        // vertical synchronisation active
        SDL_GL_SetSwapInterval(1);

        return 0;
    }

    void Window::swap_buffer()
    {
        SDL_GL_SwapWindow(p_window);
    }

    int Window::width() const
    {
        return d_width;
    }

    void Window::set_width(int width)
    {
        d_width = width;
    }

    int Window::height() const
    {
        return d_height;
    }

    void Window::set_height(int height)
    {
        d_height = height;
    }
}