

#ifndef SITEST_REGIONREPRESENTATION_HPP
#define SITEST_REGIONREPRESENTATION_HPP

#include <utility>
#include <vector>
#include <glm/glm.hpp>
#include <siren/geometry/TessellationPatch.hpp>
#include <string>

struct RegionRepresentation
{
    RegionRepresentation(std::vector<TesselationPatch*> ps, const glm::mat3x3& t, const std::string& tex_path, int cs, glm::vec4 dest_rect, const glm::vec4 _uv):
        patches(std::move(ps)),
        transform(t),
        texture_path(tex_path),
        contour_size(cs),
        destination_rect(dest_rect),
        uv(_uv)
    {}

    std::vector<TesselationPatch*> patches;
    glm::mat3x3 transform;
    std::string texture_path;
    int contour_size;
    glm::vec4 destination_rect;
    glm::vec4 uv;
};

#endif //SITEST_REGIONREPRESENTATION_HPP
