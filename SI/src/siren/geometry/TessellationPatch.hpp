

#ifndef SITEST_TESSELATIONPATCH_HPP
#define SITEST_TESSELATIONPATCH_HPP

#include <glm/glm.hpp>
#include <vector>


class TesselationPatch
{
public:
    TesselationPatch(const glm::vec3 &A, const glm::vec3 &B, const glm::vec3 &C);
    ~TesselationPatch();

    void set_abc(const glm::vec3 &A, const glm::vec3 &B, const glm::vec3 &C);
    void move(const glm::vec3 &delta);

    const glm::vec3& a() const;
    const glm::vec3& b() const;
    const glm::vec3& c() const;

    std::vector<glm::vec3> vertices();

private:
    std::vector<glm::vec3> d_vertices;
};


#endif //SITEST_TESSELATIONPATCH_HPP
