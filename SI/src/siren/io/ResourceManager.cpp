

#include "ResourceManager.hpp"

TextureCache ResourceManager::s_tex_cache;

GLTexture ResourceManager::texture(const std::string &texture_path)
{
    return s_tex_cache.texture(texture_path);
}
