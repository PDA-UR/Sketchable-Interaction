

#ifndef SITEST_GLTEXTURE_H
#define SITEST_GLTEXTURE_H

#include <GL/glew.h>

#ifdef _cplusplus
extern "C" {
#endif

typedef struct GLTexture_
{
    GLuint id;
    int width;
    int height;
} GLTexture;



#ifdef _cplusplus
}
#endif

#endif //SITEST_GLTEXTURE_H
