

#include "SpriteBatch.hpp"
#include <algorithm>

namespace SIRen
{
    SpriteBatch::SpriteBatch() :
        d_vbo(0),
        d_vao(0)
    {

    }

    SpriteBatch::~SpriteBatch()
    {
        clean();
    }

    void SpriteBatch::initialize()
    {
        create_vertex_array();
    }

    void SpriteBatch::begin(const GlyphSortType& type /* = GlyphSortType::TEXTURE */)
    {
        d_sort_type = type;

        clean();

        d_glyphs.clear();
        d_render_batches.clear();
    }

    void SpriteBatch::end()
    {
        sort_glyphs();
        create_render_batches();
    }

    void
    SpriteBatch::draw(const glm::vec4 &destination_rect, const glm::vec4 &uv_rect, GLuint texture, float depth, const Color &color)
    {
        Glyph* g = new Glyph();
        g->texture = texture;
        g->depth = depth;

        g->tlc.color = color;
        g->tlc.set_position(destination_rect.x, destination_rect.y + destination_rect.w);
        g->tlc.set_uv(uv_rect.x, uv_rect.y+ uv_rect.w);

        g->blc.color = color;
        g->blc.set_position(destination_rect.x, destination_rect.y);
        g->blc.set_uv(uv_rect.x, uv_rect.y);

        g->brc.color = color;
        g->brc.set_position(destination_rect.x + destination_rect.z, destination_rect.y);
        g->brc.set_uv(uv_rect.x + uv_rect.z, uv_rect.y);

        g->trc.color = color;
        g->trc.set_position(destination_rect.x + destination_rect.z, destination_rect.y + destination_rect.w);
        g->trc.set_uv(uv_rect.x + uv_rect.z, uv_rect.y + uv_rect.w);

        d_glyphs.push_back(g);
    }

    void SpriteBatch::render_batch()
    {
        glBindVertexArray(d_vao);

        for(auto& it : d_render_batches)
        {
            glBindTexture(GL_TEXTURE_2D, it->texture);
            glDrawArrays(GL_TRIANGLES, it->offset, it->num_vertices);
        }

        glBindVertexArray(0);
    }

    void SpriteBatch::create_render_batches()
    {
        if(d_glyphs.empty())
            return;

        std::vector<Vertex> vertices;
        vertices.resize(d_glyphs.size() * 6);

        int offset = 0;
        int current_vertex = 0;
        d_render_batches.emplace_back(new RenderBatch(offset, 6, d_glyphs[0]->texture));

        vertices[current_vertex++] = d_glyphs[0]->tlc;
        vertices[current_vertex++] = d_glyphs[0]->blc;
        vertices[current_vertex++] = d_glyphs[0]->brc;
        vertices[current_vertex++] = d_glyphs[0]->brc;
        vertices[current_vertex++] = d_glyphs[0]->trc;
        vertices[current_vertex++] = d_glyphs[0]->tlc;

        offset += 6;

        for(int current_glyph = 1; current_glyph < d_glyphs.size(); current_glyph++)
        {
            if(d_glyphs[current_glyph]->texture != d_glyphs[current_glyph - 1]->texture)
                d_render_batches.emplace_back(new RenderBatch(offset, 6, d_glyphs[current_glyph]->texture));
            else
                d_render_batches.back()->num_vertices += 6;

            vertices[current_vertex++] = d_glyphs[current_glyph]->tlc;
            vertices[current_vertex++] = d_glyphs[current_glyph]->blc;
            vertices[current_vertex++] = d_glyphs[current_glyph]->brc;
            vertices[current_vertex++] = d_glyphs[current_glyph]->brc;
            vertices[current_vertex++] = d_glyphs[current_glyph]->trc;
            vertices[current_vertex++] = d_glyphs[current_glyph]->tlc;
            offset += 6;
        }

        glBindBuffer(GL_ARRAY_BUFFER, d_vbo);

        // buffer orphaning
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void SpriteBatch::create_vertex_array()
    {
        if(!d_vao)
            glGenVertexArrays(1, &d_vao);

        glBindVertexArray(d_vao);

        if(!d_vbo)
            glGenBuffers(1, &d_vbo);

        glBindBuffer(GL_ARRAY_BUFFER, d_vbo);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        //position attribute pointer
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, position));

        //color attribute pointer
        // GL_TRUE is normalization
        glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void *) offsetof(Vertex, color));

        // uv attribute pointer
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, uv));

        glBindVertexArray(0);
    }

    void SpriteBatch::sort_glyphs()
    {
        switch (d_sort_type)
        {
            case GlyphSortType::BACK_TO_FRONT:
                std::stable_sort(d_glyphs.begin(), d_glyphs.end(), compare_back_to_front);

                break;

            case GlyphSortType::FRONT_TO_BACK:
                std::stable_sort(d_glyphs.begin(), d_glyphs.end(), compare_front_to_back);

                break;

            case GlyphSortType::TEXTURE:
                std::stable_sort(d_glyphs.begin(), d_glyphs.end(), compare_texture);

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

    void SpriteBatch::clean()
    {
        while(!d_glyphs.empty())
        {
            delete d_glyphs.back();
            d_glyphs.pop_back();
        }

        while(!d_render_batches.empty())
        {
            delete d_render_batches.back();
            d_render_batches.pop_back();
        }
    }
}
