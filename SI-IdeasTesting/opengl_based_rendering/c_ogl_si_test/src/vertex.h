
#ifndef C_OGL_SI_TEST_VERTEX_H
#define C_OGL_SI_TEST_VERTEX_H

#include "GL/glew.h"

typedef struct
{
    float x, y;
} Position;

typedef struct
{
    GLubyte r;
    GLubyte g;
    GLubyte b;
    GLubyte a;
} Color;

typedef struct
{
    Position position;
    Color color;

} Vertex;

#endif //C_OGL_SI_TEST_VERTEX_H
