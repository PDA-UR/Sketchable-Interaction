

#include "TessellationPatch.hpp"

TesselationPatch::TesselationPatch(const glm::vec3 &A, const glm::vec3 &B, const glm::vec3 &C):
        d_vertices(std::vector<glm::vec3>{A, B, C})
{

}

TesselationPatch::~TesselationPatch()
{

}

void TesselationPatch::set_abc(const glm::vec3 &A, const glm::vec3 &B, const glm::vec3 &C)
{
    d_vertices = std::vector<glm::vec3> {A, B, C};
}

void TesselationPatch::move(const glm::vec3 &delta)
{
    for(auto& v : d_vertices)
    {
        v += delta;
        v.z = 1;
    }
}

const glm::vec3& TesselationPatch::a() const
{
    return d_vertices[0];
}

const glm::vec3& TesselationPatch::b() const
{
    return d_vertices[1];
}

const glm::vec3& TesselationPatch::c() const
{
    return d_vertices[2];
}

std::vector<glm::vec3> TesselationPatch::vertices()
{
    return d_vertices;
}