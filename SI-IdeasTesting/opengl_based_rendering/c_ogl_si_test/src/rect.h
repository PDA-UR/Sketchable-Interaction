
#ifndef C_OGL_SI_TEST_RECT_H
#define C_OGL_SI_TEST_RECT_H

#include <GL/glew.h>
#include "vertex.h"

typedef struct
{
    float x, y, width, height;
    GLuint vboID;

} Rect;

Rect rect_construct(float x, float y, float width, float height);
void rect_destruct(Rect* rect);

void rect_render(Rect* rect);

#endif //C_OGL_SI_TEST_RECT_H
