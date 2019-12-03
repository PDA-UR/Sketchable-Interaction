

#ifndef SITEST_IMAGELOADER_HPP
#define SITEST_IMAGELOADER_HPP

#include <siren/geometry/GLTexture.hpp>

class ImageLoader
{
public:
    static GLTexture load_png(const char *filepath);
};

#endif //SITEST_IMAGELOADER_HPP
