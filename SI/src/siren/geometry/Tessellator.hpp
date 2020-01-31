

#ifndef SITEST_TESSELLATOR_HPP
#define SITEST_TESSELLATOR_HPP

#include <vector>
#include <GL/glew.h>
#include <GL/glu.h>
#include <glm/glm.hpp>

#ifndef TESSELATION_CALLBACK
#define TESSELATION_CALLBACK
#endif

class Tessellator
{
public:
    static bool tesselate(std::vector<glm::vec3> &out, const std::vector<glm::vec3> &vertices);

private:
    static TESSELATION_CALLBACK void vertex_data(const GLvoid *data);

    static TESSELATION_CALLBACK void edge_flag(GLboolean is_set);

    static TESSELATION_CALLBACK void vertex_combine(const GLdouble new_vertex[3], const GLdouble *neighbour_vertex[4], const GLfloat neighbour_weight[4], GLdouble **out);

    static std::vector<glm::vec3> temp;
    static int vertex_index;
    static std::vector<std::vector<GLdouble>> d_vertices;
};


#endif //SITEST_TESSELLATOR_HPP
