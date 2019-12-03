

#ifndef SITEST_VERTEX_HPP
#define SITEST_VERTEX_HPP

#include <GL/glew.h>

// use 8 bytes for vertex position
struct Position
{
    float x;
    float y;
};

// use 4 bytes for color
struct Color
{
    Color() {}
    Color(GLubyte R, GLubyte G, GLubyte B, GLubyte A) :
            r(R), g(G), b(B), a(A)
    {}

    GLubyte r;
    GLubyte g;
    GLubyte b;
    GLubyte a;
};

// use 8 bytes for texture uv coords
struct UV
{
    float u;
    float v;
};

struct Vertex
{
    struct Position position;
    struct Color color;
    struct UV uv;

    void set_position(float x, float y)
    {
        position.x = x;
        position.y = y;
    }

    void set_uv(float u, float v)
    {
        uv.u = u;
        uv.v = v;
    }

    void set_color(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
    {
        color.r = r;
        color.g = g;
        color.b = b;
        color.a = a;
    }

};

#endif //SITEST_VERTEX_HPP
