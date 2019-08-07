

#ifndef SITEST_VERTEX_H
#define SITEST_VERTEX_H

#ifdef __cplusplus
extern "C" {
#endif

#include <GL/glew.h>

struct Position
{
    float x;
    float y;
};

// use 4 bytes for color
struct Color
{
    GLubyte r;
    GLubyte g;
    GLubyte b;
    GLubyte a;
};

typedef struct Vertex_
{
    struct Position position;
    struct Color color;
} Vertex;

#ifdef __cplusplus
}
#endif

#endif //SITEST_VERTEX_H
