

#ifndef SITEST_REGIONSPRITE_HPP
#define SITEST_REGIONSPRITE_HPP

#include <GL/glew.h>
#include <render_engine/sdl2/geometry/GLTexture.hpp>
#include <string>

namespace SIRen
{
    class RegionSprite
    {
    public:
        RegionSprite() = default;

        RegionSprite(int x, int y, int width, int height, const std::string &texture_path);

        ~RegionSprite();

        void initialize(int x, int y, int width, int height, const std::string &texture_path);

        void draw();

    private:
        int d_x, d_y, d_width, d_height;
        GLuint d_vbo_id;
        GLTexture d_texture;
    };
}

#endif //SITEST_REGIONSPRITE_HPP
