

#ifndef SITEST_REGIONSPRITE_HPP
#define SITEST_REGIONSPRITE_HPP

#include <GL/glew.h>
#include <render_engine/sdl2/shader/GLTexture.hpp>
#include <string>

class RegionSprite
{
public:
    RegionSprite(int x, int y, int width, int height, const std::string& texture_path);
    ~RegionSprite();

    void draw();

private:
    int d_x, d_y, d_width, d_height;
    GLuint d_vbo_id;
    GLTexture d_texture;
};

void region_sprite_initialize(RegionSprite* rs, int x, int y, int width, int height, const char* texture_path);
void region_sprite_draw(RegionSprite* rs);
void region_sprite_destroy(RegionSprite* rs);

#endif //SITEST_REGIONSPRITE_HPP
