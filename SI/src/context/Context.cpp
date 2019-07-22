

#include "Context.hpp"


Context::Context(int width, int height) : d_width(width), d_height(height)
{
    // check if is initial start, therefore create the first layer and add it to layermanager
}

Context::~Context()
{

}

int Context::width() const
{
    return d_width;
}

void Context::set_width(int width)
{
    d_width = width;
}

int Context::height() const
{
    return d_height;
}

void Context::set_height(int height)
{
    d_height = height;
}
