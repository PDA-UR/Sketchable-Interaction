
#include "RegionSprite.hpp"
#include "render_engine/sdl2/geometry/Vertex.hpp"
#include <cstddef>
#include <render_engine/sdl2/io/ResourceManager.hpp>

namespace SIRen
{
    RegionSprite::RegionSprite(int x, int y, int width, int height, const std::string &texture_path)
    {
        initialize(x, y, width, height, texture_path);
    }

    RegionSprite::~RegionSprite()
    {
        if (d_vbo_id != 0)
            glDeleteBuffers(1, &d_vbo_id);
    }

    void RegionSprite::initialize(int x, int y, int width, int height, const std::string &texture_path)
    {
        d_x = x;
        d_y = y;
        d_width = width;
        d_height = height;
        d_texture = ResourceManager::texture(texture_path);

        glGenBuffers(1, &d_vbo_id);

        Vertex vertex_data[6];

        vertex_data[0].set_position(x + width, y + height);
        vertex_data[0].set_uv(1, 1);

        vertex_data[1].set_position(x, y + height);
        vertex_data[1].set_uv(0, 1);

        vertex_data[2].set_position(x, y);
        vertex_data[2].set_uv(0, 0);

        vertex_data[3].set_position(x, y);
        vertex_data[3].set_uv(0, 0);

        vertex_data[4].set_position(x + width, y);
        vertex_data[4].set_uv(1, 0);

        vertex_data[5].set_position(x + width, y + height);
        vertex_data[5].set_uv(1, 1);

        for (int i = 0; i < 6; i++)
            vertex_data[i].set_color(255, 0, 255, 255);

        glBindBuffer(GL_ARRAY_BUFFER, d_vbo_id);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void RegionSprite::draw()
    {
        glBindTexture(GL_TEXTURE_2D, d_texture.id);

        glBindBuffer(GL_ARRAY_BUFFER, d_vbo_id);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}
