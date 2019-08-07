

#ifndef SITEST_IMAGELOADER_HPP
#define SITEST_IMAGELOADER_HPP

#include "GLTexture.h"

class ImageLoader
{
public:
    GLTexture load_png(const char* filepath);
};


#endif //SITEST_IMAGELOADER_HPP
