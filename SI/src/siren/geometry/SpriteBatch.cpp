

#include <algorithm>
#include "SpriteBatch.hpp"
#include <vector>
#include <memory>
#include <glm/glm.hpp>

SpriteBatch::SpriteBatch() :
        d_sort_type(GlyphSortType::FRONT_TO_BACK),
        d_shader_area(std::make_shared<GLSLProgram>()),
        d_shader_texture(std::make_shared<GLSLProgram>())
{

}

SpriteBatch::~SpriteBatch()
{
    if(d_vbos[0] && d_vbos[1])
        glDeleteBuffers(2, d_vbos);

    if(d_vaos[0] && d_vaos[1])
        glDeleteBuffers(2, d_vaos);
}

void SpriteBatch::initialize()
{
    d_shader_area->compile_shaders("src/siren/res/shaders/region/region_area.vert", "src/siren/res/shaders/region/region_area.frag");
    d_shader_area->add_attribute("vertex_position");
    d_shader_area->add_attribute("vertex_color");
    d_shader_area->link_shaders();

    d_shader_texture->compile_shaders("src/siren/res/shaders/region/region_texture.vert","src/siren/res/shaders/region/region_texture.frag");
    d_shader_texture->add_attribute("vertex_position");
    d_shader_texture->add_attribute("vertex_color");
    d_shader_texture->add_attribute("vertex_uv");
    d_shader_texture->link_shaders();

    create_vertex_arrays();
}

void SpriteBatch::register_region_sprites(const std::vector<std::shared_ptr<Region>> &sprites, const GLfloat *camera_matrix)
{
    if(!sprites.empty())
    {
        d_render_batches.clear();

//        d_step_count = sprites[0]->contour_size();

        begin_area(camera_matrix);
        draw_area(sprites);
        end_area();

        begin_texture(camera_matrix);
        draw_texture(sprites);
        end_texture();

        create_render_batches();
    }
}

void SpriteBatch::register_partial_region_sprite(const std::vector<glm::vec2> &partial_region, const GLfloat *camera_matrix)
{
    std::vector<Vertex> vertices;

    for(auto& p : partial_region)
    {
        vertices.emplace_back();
        vertices.back().set_position(p.x, p.y);
        vertices.back().set_color(255, 255, 255, 255);
    }

    d_shader_area->use();
    glUniformMatrix4fv(d_shader_area->uniform_location("P"), 1, GL_FALSE, camera_matrix);
    glBindBuffer(GL_ARRAY_BUFFER, d_vbos[2]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    d_shader_area->unuse();
}

void SpriteBatch::render(const std::vector<std::shared_ptr<Region>> &sprites, const std::vector<glm::vec2>& partial_contour, const GLfloat *camera_matrix)
{
    register_region_sprites(sprites, camera_matrix);
    register_partial_region_sprite(partial_contour, camera_matrix);

    for (auto &batch : d_render_batches)
    {
        d_shader_area->use();
        glBindVertexArray(d_vaos[0]);
        glDrawArrays(GL_TRIANGLES, batch.offset, batch.num_vertices);
        d_shader_area->unuse();

        d_shader_texture->use();
        glBindVertexArray(d_vaos[1]);
        glBindTexture(GL_TEXTURE_2D, batch.texture);
        glDrawArrays(GL_TRIANGLES, batch.offset2, batch.num_vertices2);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
        d_shader_texture->unuse();
    }

    d_shader_area->use();
    glBindVertexArray(d_vaos[2]);

    glDrawArrays(GL_LINE_STRIP, 0, partial_contour.size());

    glBindVertexArray(0);
    d_shader_area->unuse();
}

void SpriteBatch::create_vertex_arrays()
{
    glGenVertexArrays(3, d_vaos);
    glGenBuffers(3, d_vbos);

    //area
    glBindVertexArray(d_vaos[0]);
    glBindBuffer(GL_ARRAY_BUFFER, d_vbos[0]);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    //position attribute pointer
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, position));

    //color attribute pointer
    // GL_TRUE is normalization
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void *) offsetof(Vertex, color));

    // texture
    glBindVertexArray(d_vaos[1]);
    glBindBuffer(GL_ARRAY_BUFFER, d_vbos[1]);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    //position attribute pointer
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, position));

    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void *) offsetof(Vertex, color));

    // uv attribute pointer
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, uv));

    // partial region
    glBindVertexArray(d_vaos[2]);
    glBindBuffer(GL_ARRAY_BUFFER, d_vbos[2]);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, position));
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void *) offsetof(Vertex, color));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void SpriteBatch::sort_glyphs(std::vector<Glyph *> &glyphs)
{
    switch (d_sort_type)
    {
        case GlyphSortType::BACK_TO_FRONT:
            std::stable_sort(glyphs.begin(), glyphs.end(), compare_back_to_front);

            break;

        case GlyphSortType::FRONT_TO_BACK:
            std::stable_sort(glyphs.begin(), glyphs.end(), compare_front_to_back);

            break;

        case GlyphSortType::TEXTURE:
            std::stable_sort(glyphs.begin(), glyphs.end(), compare_texture);

            break;

        default:
            break;
    }
}

bool SpriteBatch::compare_front_to_back(const Glyph *a, const Glyph *b)
{
    return a->depth < b->depth;
}

bool SpriteBatch::compare_back_to_front(const Glyph *a, const Glyph *b)
{
    return a->depth > b->depth;
}

bool SpriteBatch::compare_texture(const Glyph *a, const Glyph *b)
{
    return a->texture < b->texture;
}

void SpriteBatch::set_draw_mode(GLenum mode)
{
    glPolygonMode(GL_FRONT, mode);
}

void SpriteBatch::begin_area(const GLfloat *camera_matrix, const GlyphSortType &type)
{
    d_shader_area->use();

    glUniformMatrix4fv(d_shader_area->uniform_location("P"), 1, GL_FALSE, camera_matrix);

    d_sort_type = type;
    d_glyphs_area.clear();
}

void SpriteBatch::draw_area(const std::vector<std::shared_ptr<Region>> &region_sprites)
{
    for (const auto &region : region_sprites)
    {
        d_glyphs_area.emplace_back(region);
        d_glyphs_area.back().depth = cummulative_depth++;
    }
}

void SpriteBatch::end_area()
{
    d_glyphs_area_ptr.resize(d_glyphs_area.size());

    for (int i = 0; i < d_glyphs_area.size(); i++)
        d_glyphs_area_ptr[i] = &d_glyphs_area[i];

    sort_glyphs(d_glyphs_area_ptr);
    d_shader_area->unuse();
}

void SpriteBatch::begin_texture(const GLfloat *camera_matrix, const GlyphSortType &type)
{
    d_shader_texture->use();
    glActiveTexture(GL_TEXTURE0);
    glUniformMatrix4fv(d_shader_texture->uniform_location("P"), 1, GL_FALSE,
                       static_cast<const GLfloat *>(camera_matrix));
    glUniform1i(d_shader_texture->uniform_location("mysampler"), 0);

    d_glyphs_texture.clear();
    d_sort_type = type;
}

void SpriteBatch::draw_texture(const std::vector<std::shared_ptr<Region>> &region_sprites)
{
//    for (const auto &region_sprite : region_sprites)
//        d_glyphs_texture.emplace_back(region_sprite->texture_rectangle(), region_sprite->texture_uv(),
//                                      region_sprite->texture_path(), cummulative_depth++, Color(255, 255, 255, 255));
}

void SpriteBatch::end_texture()
{
    d_glyphs_texture_ptr.resize(d_glyphs_texture.size());

    for (int i = 0; i < d_glyphs_area.size(); i++)
        d_glyphs_texture_ptr[i] = &d_glyphs_texture[i];

    sort_glyphs(d_glyphs_texture_ptr);
    d_shader_texture->unuse();
}

void SpriteBatch::create_render_batches()
{
    if (d_glyphs_area.empty() || d_glyphs_texture_ptr.empty() ||
        (d_glyphs_area_ptr.size() != d_glyphs_texture_ptr.size()))
        return;

    std::vector<Vertex> vertices_area, vertices_texture;

    vertices_area.resize(d_glyphs_area_ptr.size() * d_glyphs_area_ptr[0]->vertices.size());
    vertices_texture.resize(d_glyphs_texture_ptr.size() * 6);

    int offset_area = 0;
    int offset_texture = 0;
    int current_vertex_area = 0;
    int current_vertex_texture = 0;

    d_render_batches.emplace_back(offset_area, d_glyphs_area_ptr[0]->vertices.size(), offset_texture, 6,
                                  d_glyphs_texture_ptr[0]->texture);

    vertices_texture[current_vertex_texture++] = d_glyphs_texture_ptr[0]->tlc;
    vertices_texture[current_vertex_texture++] = d_glyphs_texture_ptr[0]->blc;
    vertices_texture[current_vertex_texture++] = d_glyphs_texture_ptr[0]->brc;
    vertices_texture[current_vertex_texture++] = d_glyphs_texture_ptr[0]->brc;
    vertices_texture[current_vertex_texture++] = d_glyphs_texture_ptr[0]->trc;
    vertices_texture[current_vertex_texture++] = d_glyphs_texture_ptr[0]->tlc;

    offset_texture += 6;

    for (int i = 0; i < d_glyphs_area_ptr[0]->vertices.size(); i++)
        vertices_area[current_vertex_area++] = d_glyphs_area_ptr[0]->vertices[i];

    offset_area += d_glyphs_area_ptr[0]->vertices.size();

    for (int i = 1; i < d_glyphs_area.size(); i++)
    {
        if (d_glyphs_texture_ptr[i]->texture != d_glyphs_texture_ptr[i - 1]->texture)
            d_render_batches.emplace_back(offset_area, d_glyphs_area_ptr[i]->vertices.size(), offset_texture, 6,
                                          d_glyphs_texture_ptr[i]->texture);
        else
        {
            d_render_batches.back().num_vertices2 += 6;
            d_render_batches.back().num_vertices += d_glyphs_area_ptr[i]->vertices.size();
        }

        vertices_texture[current_vertex_texture++] = d_glyphs_texture_ptr[i]->tlc;
        vertices_texture[current_vertex_texture++] = d_glyphs_texture_ptr[i]->blc;
        vertices_texture[current_vertex_texture++] = d_glyphs_texture_ptr[i]->brc;
        vertices_texture[current_vertex_texture++] = d_glyphs_texture_ptr[i]->brc;
        vertices_texture[current_vertex_texture++] = d_glyphs_texture_ptr[i]->trc;
        vertices_texture[current_vertex_texture++] = d_glyphs_texture_ptr[i]->tlc;
        offset_texture += 6;

        for (int k = 0; k < d_glyphs_area_ptr[i]->vertices.size(); k++)
            vertices_area[current_vertex_area++] = d_glyphs_area_ptr[i]->vertices[k];

        offset_area += d_glyphs_area_ptr[i]->vertices.size();
    }

    glBindBuffer(GL_ARRAY_BUFFER, d_vbos[0]);

    // buffer orphaning
    glBufferData(GL_ARRAY_BUFFER, vertices_area.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices_area.size() * sizeof(Vertex), vertices_area.data());

    glBindBuffer(GL_ARRAY_BUFFER, d_vbos[1]);

    // buffer orphaning
    glBufferData(GL_ARRAY_BUFFER, vertices_texture.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices_texture.size() * sizeof(Vertex), vertices_texture.data());

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}