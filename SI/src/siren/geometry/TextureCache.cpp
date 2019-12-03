

#include <siren/io/ImageLoader.hpp>
#include "TextureCache.hpp"

TextureCache::TextureCache()
{

}

TextureCache::~TextureCache()
{

}

GLTexture TextureCache::texture(const std::string &texture_path)
{
    auto it = d_texture_map.find(texture_path);

    if (it == d_texture_map.end())
    {
        GLTexture tex = ImageLoader::load_png(texture_path.c_str());
        d_texture_map.insert(std::make_pair(texture_path, tex));

        return tex;
    }

    return it->second;
}
