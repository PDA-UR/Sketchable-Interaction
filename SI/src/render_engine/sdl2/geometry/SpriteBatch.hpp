

#ifndef SITEST_SPRITEBATCH_HPP
#define SITEST_SPRITEBATCH_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

#include "Vertex.hpp"
#include "Glyph.hpp"

namespace SIRen
{
    class RenderBatch
    {
    public:
        RenderBatch(GLuint off, GLuint vertices, GLuint tex) :
            offset(off),
            num_vertices(vertices),
            texture(tex)
        {}

        GLuint offset;
        GLuint num_vertices;
        GLuint texture;

    private:
    };

    class SpriteBatch
    {
    public:
        SpriteBatch();
        ~SpriteBatch();

        void initialize();
        void begin(const GlyphSortType& type = GlyphSortType::TEXTURE);
        void end();
        void draw(const glm::vec4& destination_rect, const glm::vec4& uv_rect, GLuint texture, float depth, const Color& color);
        void render_batch();

    private:
        void create_render_batches();
        void create_vertex_array();
        void sort_glyphs();

        static bool compare_front_to_back(const Glyph* a, const Glyph* b);
        static bool compare_back_to_front(const Glyph* a, const Glyph* b);
        static bool compare_texture(const Glyph* a, const Glyph* b);

        void clean();

        GLuint d_vbo;
        GLuint d_vao;

        GlyphSortType d_sort_type;

        std::vector<Glyph*> d_glyphs;
        std::vector<RenderBatch*> d_render_batches;
    };
}

#endif //SITEST_SPRITEBATCH_HPP
