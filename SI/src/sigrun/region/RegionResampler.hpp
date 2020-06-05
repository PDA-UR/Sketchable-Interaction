

#ifndef SITEST_REGIONRESAMPLER_HPP
#define SITEST_REGIONRESAMPLER_HPP

#include <vector>
#include <glm/glm.hpp>

#define STEPCOUNT 100

class RegionResampler
{
public:
    static void resample(std::vector<glm::vec3>& out, const std::vector<glm::vec3>& in, int step_count=STEPCOUNT);

private:
    static float distance(const glm::vec3& p, const glm::vec3& q);
    static float total_length(const std::vector<glm::vec3>& vertices);

    friend class SIGRunRegionResamplerTest;
};


#endif //SITEST_REGIONRESAMPLER_HPP
