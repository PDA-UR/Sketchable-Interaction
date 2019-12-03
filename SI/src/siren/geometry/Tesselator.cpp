

#include "Tesselator.hpp"

#include "Tesselator.hpp"

std::vector<glm::vec2> Tesselator::temp;
int Tesselator::vertex_index = 0;
std::vector<std::vector<GLdouble>> Tesselator::d_vertices;

bool Tesselator::tesselate(std::vector<glm::vec2> &out, const std::vector<glm::vec2> &vertices)
{
    GLUtesselator *tess = gluNewTess();

    gluTessCallback(tess, GLU_TESS_VERTEX, (void (TESSELATION_CALLBACK *)()) Tesselator::vertex_data);
    gluTessCallback(tess, GLU_TESS_EDGE_FLAG, (void (TESSELATION_CALLBACK *)()) Tesselator::edge_flag);
    gluTessCallback(tess, GLU_TESS_COMBINE, (void (TESSELATION_CALLBACK *)()) Tesselator::vertex_combine);

    gluTessProperty(tess, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_NONZERO);
    gluTessBeginPolygon(tess, nullptr);
    gluTessBeginContour(tess);

    GLdouble coords[vertices.size()][3];

    for (size_t i = 0; i < vertices.size(); i++)
    {
        coords[i][0] = vertices[i].x;
        coords[i][1] = vertices[i].y;
        coords[i][2] = 0;

        gluTessVertex(tess, coords[i], coords[i]);
    }

    gluTessEndContour(tess);
    gluTessEndPolygon(tess);

    gluDeleteTess(tess);

    vertex_index = 0;
    d_vertices.clear();

    if (temp.empty())
        return false;

    out = temp;
    temp.clear();

    return true;
}

TESSELATION_CALLBACK void Tesselator::vertex_data(const GLvoid *data)
{
    temp.emplace_back(((GLdouble *) data)[0], ((GLdouble *) data)[1]);
}

TESSELATION_CALLBACK void Tesselator::edge_flag(GLboolean is_set)
{
    // forcing Tesselator to output GL_TRIANGLES
}

// does not work if start and end are within the polygon to be combined

TESSELATION_CALLBACK void
Tesselator::vertex_combine(const GLdouble new_vertex[3], const GLdouble *neighbour_vertex[4],
                           const GLfloat neighbour_weight[4], GLdouble **out)
{
    d_vertices.emplace_back(6);

    d_vertices[vertex_index][0] = new_vertex[0];
    d_vertices[vertex_index][1] = new_vertex[1];
    d_vertices[vertex_index][2] = new_vertex[2];

    // compute vertex color with given weights and colors of 4 neighbors
    // the neighborVertex[4] must hold required info, in this case, color.
    // neighborVertex was actually the third param of gluTessVertex() and is
    // passed into here to compute the color of the intersect vertex.
    d_vertices[vertex_index][3] = neighbour_weight[0] * neighbour_vertex[0][3] +   // red
                                  neighbour_weight[1] * neighbour_vertex[1][3] +
                                  neighbour_weight[2] * neighbour_vertex[2][3] +
                                  neighbour_weight[3] * neighbour_vertex[3][3];
    d_vertices[vertex_index][4] = neighbour_weight[0] * neighbour_vertex[0][4] +   // green
                                  neighbour_weight[1] * neighbour_vertex[1][4] +
                                  neighbour_weight[2] * neighbour_vertex[2][4] +
                                  neighbour_weight[3] * neighbour_vertex[3][4];
    d_vertices[vertex_index][5] = neighbour_weight[0] * neighbour_vertex[0][5] +   // blue
                                  neighbour_weight[1] * neighbour_vertex[1][5] +
                                  neighbour_weight[2] * neighbour_vertex[2][5] +
                                  neighbour_weight[3] * neighbour_vertex[3][5];


    // return output data (vertex coords and others)
    *out = d_vertices[vertex_index].data();   // assign the address of new intersect vertex

    ++vertex_index;  // increase index for next vertex
}