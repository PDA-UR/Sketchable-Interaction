

#ifndef SITEST_SIGRUNREGIONMASKTEST_HPP
#define SITEST_SIGRUNREGIONMASKTEST_HPP

#include <gtest/gtest.h>
#include <vector>
#include <glm/glm.hpp>


class SIGRunRegionMaskTest: public ::testing::Test
{
public:
    virtual void SetUp()
    {
        std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

        d_rm = new RegionMask(1920, 1080, contour, contour);
    }

    virtual void TearDown()
    {
        delete d_rm;
        d_rm = nullptr;
    }

    RegionMask* d_rm;

protected:
    void scanlinefill(const std::vector<glm::vec3>& contour, const std::vector<glm::vec3>& aabb)
    {
        d_rm->scanlinefill(contour, aabb);
    }
};


#endif //SITEST_SIGRUNREGIONMASKTEST_HPP
