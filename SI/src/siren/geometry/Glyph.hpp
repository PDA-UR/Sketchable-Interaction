
#ifndef SITEST_GLYPH_HPP
#define SITEST_GLYPH_HPP

#include <GL/glew.h>
#include "Vertex.hpp"
#include <vector>
#include <SI/SI.hpp>
#include <siren/io/ResourceManager.hpp>

enum class GlyphSortType
{
    NONE,
    FRONT_TO_BACK,
    BACK_TO_FRONT,
    TEXTURE
};

class Glyph
{
public:
    Glyph()
    {}

    Glyph(const std::shared_ptr<Region>& region):
            depth(0), texture(ResourceManager::texture(region->texture_path()).id)
    {
//        for (const auto &patch : region->patches())
//        {
//            vertices.emplace_back();
//            vertices.back().set_position(patch.a().x, patch.a().y);
//            vertices.back().set_color(region->color().r, region->color().g, region->color().b, region->color().a);
//
//            vertices.emplace_back();
//            vertices.back().set_position(patch.b().x, patch.b().y);
//            vertices.back().set_color(region->color().r, region->color().g, region->color().b, region->color().a);
//
//            vertices.emplace_back();
//            vertices.back().set_position(patch.c().x, patch.c().y);
//            vertices.back().set_color(region->color().r, region->color().g, region->color().b, region->color().a);
//        }
    }

    Glyph(const glm::vec4 &destination_rect, const glm::vec4 &uv_rect, const std::string& tex, float d, const Color &color) :
            texture(ResourceManager::texture(tex).id), depth(d)
    {
        tlc.color = color;
        tlc.set_position(destination_rect.x, destination_rect.y + destination_rect.w);
        tlc.set_uv(uv_rect.x, uv_rect.y + uv_rect.w);

        blc.color = color;
        blc.set_position(destination_rect.x, destination_rect.y);
        blc.set_uv(uv_rect.x, uv_rect.y);

        brc.color = color;
        brc.set_position(destination_rect.x + destination_rect.z, destination_rect.y);
        brc.set_uv(uv_rect.x + uv_rect.z, uv_rect.y);

        trc.color = color;
        trc.set_position(destination_rect.x + destination_rect.z, destination_rect.y + destination_rect.w);
        trc.set_uv(uv_rect.x + uv_rect.z, uv_rect.y + uv_rect.w);
    }

    GLuint texture;
    float depth;

    std::vector<Vertex> vertices;

    Vertex tlc, blc, brc, trc;
};

#endif //SITEST_GLYPH_HPP
