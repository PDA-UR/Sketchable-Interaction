

#ifndef SITEST_RESOURCEMANAGER_HPP
#define SITEST_RESOURCEMANAGER_HPP

#include <string>
#include <siren/geometry/GLTexture.hpp>
#include <siren/geometry/TextureCache.hpp>

class ResourceManager
{
public:
    static GLTexture texture(const std::string &texture_path);

private:
    static TextureCache s_tex_cache;
};

#endif //SITEST_RESOURCEMANAGER_HPP
