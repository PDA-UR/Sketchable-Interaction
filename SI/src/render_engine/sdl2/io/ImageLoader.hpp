

#ifndef SITEST_IMAGELOADER_HPP
#define SITEST_IMAGELOADER_HPP

#include "render_engine/sdl2/geometry/GLTexture.hpp"

namespace SIRen
{
    class ImageLoader
    {
    public:
        static GLTexture load_png(const char *filepath);
    };
}

#endif //SITEST_IMAGELOADER_HPP
