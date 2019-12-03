

#ifndef SITEST_TESSELATOR_HPP
#define SITEST_TESSELATOR_HPP

#include <vector>
#include <GL/glew.h>
#include <GL/glu.h>
#include <glm/glm.hpp>

#ifndef TESSELATION_CALLBACK
#define TESSELATION_CALLBACK
#endif

class Tesselator
{
public:
    static bool tesselate(std::vector<glm::vec2> &out, const std::vector<glm::vec2> &vertices);

private:
    static TESSELATION_CALLBACK void vertex_data(const GLvoid *data);

    static TESSELATION_CALLBACK void edge_flag(GLboolean is_set);

    static TESSELATION_CALLBACK void vertex_combine(const GLdouble new_vertex[3], const GLdouble *neighbour_vertex[4], const GLfloat neighbour_weight[4], GLdouble **out);

    static std::vector<glm::vec2> temp;
    static int vertex_index;
    static std::vector<std::vector<GLdouble>> d_vertices;
};


#endif //SITEST_TESSELATOR_HPP
