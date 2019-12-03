

#ifndef SITEST_SIGRUNREGIONRESAMPLERTEST_HPP
#define SITEST_SIGRUNREGIONRESAMPLERTEST_HPP

#include <gtest/gtest.h>
#include <glm/glm.hpp>
#include <sigrun/region/RegionResampler.hpp>

class SIGRunRegionResamplerTest: public ::testing::Test
{
public:
    void SetUp()
    {

    }

    void TearDown()
    {

    }

    float rr_distance(const glm::vec3& p, const glm::vec3& q)
    {
        return RegionResampler::distance(p, q);
    }

    float rr_total_length(const std::vector<glm::vec3>& vertices)
    {
        return RegionResampler::total_length(vertices);
    }
};


#endif //SITEST_SIGRUNREGIONRESAMPLERTEST_HPP
