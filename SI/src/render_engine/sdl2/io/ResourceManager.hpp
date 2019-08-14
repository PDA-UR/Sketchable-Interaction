

#ifndef SITEST_RESOURCEMANAGER_HPP
#define SITEST_RESOURCEMANAGER_HPP


#include <render_engine/sdl2/geometry/TextureCache.hpp>

namespace SIRen
{
    class ResourceManager
    {
    public:
        static GLTexture texture(const std::string &texture_path);

    private:
        static TextureCache s_tex_cache;
    };
}

#endif //SITEST_RESOURCEMANAGER_HPP
