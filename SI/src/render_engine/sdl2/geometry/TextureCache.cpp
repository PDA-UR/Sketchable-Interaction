

#include <render_engine/sdl2/io/ImageLoader.hpp>
#include <core/debug/Print.hpp>
#include <render_engine/debug/SiRenPrint.hpp>
#include "TextureCache.hpp"

namespace SIRen
{
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
}
