

#ifndef SITEST_TEXTURECACHE_HPP
#define SITEST_TEXTURECACHE_HPP

#include <map>
#include "GLTexture.hpp"

namespace SIRen
{
    class TextureCache
    {
    public:
        TextureCache();

        ~TextureCache();

        GLTexture texture(const std::string &texture_path);

    private:
        std::map<std::string, GLTexture> d_texture_map;
    };
}

#endif //SITEST_TEXTURECACHE_HPP
