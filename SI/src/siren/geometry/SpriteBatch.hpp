

#ifndef SITEST_SPRITEBATCH_HPP
#define SITEST_SPRITEBATCH_HPP


#include <GL/glew.h>
#include <vector>
#include <memory>
#include "Glyph.hpp"

class RenderBatch
{
public:
    GLuint offset;
    GLuint num_vertices;
    GLuint offset2;
    GLuint num_vertices2;
    GLuint texture;

    RenderBatch(GLuint off, GLuint vertices, GLuint off2, GLuint vertices2, GLuint tex) :
            offset(off), num_vertices(vertices), offset2(off2), num_vertices2(vertices2), texture(tex)
    {}
};

class SpriteBatch
{
public:
    SpriteBatch();
    ~SpriteBatch();

    void initialize();
    void render(const std::map<std::string, RegionRepresentation*>& regions, const std::vector<glm::vec2>& partial_contour, const GLfloat *camera_matrix);
    void set_draw_mode(GLenum mode);

private:
    void register_region_sprites(const std::map<std::string, RegionRepresentation*>& regions, const GLfloat *camera_matrix);
    void register_partial_region_sprite(const std::vector<glm::vec2>& partial_region, const GLfloat* camera_matrix);
    void begin_area(const GLfloat *camera_matrix, const GlyphSortType &type = GlyphSortType::FRONT_TO_BACK);
    void draw_area(const std::map<std::string, RegionRepresentation*>& regions);
    void end_area();
    void begin_texture(const GLfloat *camera_matrix, const GlyphSortType &type = GlyphSortType::FRONT_TO_BACK);
    void draw_texture(const std::map<std::string, RegionRepresentation*>& regions);
    void end_texture();
    void create_render_batches();
    void create_vertex_arrays();
    void sort_glyphs(std::vector<Glyph *> &glyphs);

    static bool compare_front_to_back(const Glyph *a, const Glyph *b);
    static bool compare_back_to_front(const Glyph *a, const Glyph *b);
    static bool compare_texture(const Glyph *a, const Glyph *b);

    GLuint d_vbos[3], d_vaos[3];

    int d_step_count;

    std::shared_ptr<GLSLProgram> d_shader_area, d_shader_texture;
    GlyphSortType d_sort_type;

    std::vector<Glyph> d_glyphs_area, d_glyphs_texture;
    std::vector<Glyph *> d_glyphs_area_ptr, d_glyphs_texture_ptr;
    std::vector<RenderBatch> d_render_batches;

    long cummulative_depth = 0;
};

#endif //SITEST_SPRITEBATCH_HPP
