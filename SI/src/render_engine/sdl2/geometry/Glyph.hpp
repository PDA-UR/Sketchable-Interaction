

#ifndef SITEST_GLYPH_HPP
#define SITEST_GLYPH_HPP


#include <GL/glew.h>
#include "Vertex.hpp"

namespace SIRen
{
    enum class GlyphSortType
    {
        NONE,
        FRONT_TO_BACK,
        BACK_TO_FRONT,
        TEXTURE
    };

    struct Glyph
    {
        GLuint texture;
        float depth;

        Vertex tlc;
        Vertex blc;
        Vertex brc;
        Vertex trc;
    };
}

#endif //SITEST_GLYPH_HPP
