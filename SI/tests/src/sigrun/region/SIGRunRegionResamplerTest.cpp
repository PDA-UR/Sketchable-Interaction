

#include <sigrun/plugin/PluginCollector.hpp>
#include <sigrun/plugin/Scripting.hpp>
#include <sigrun/region/Region.hpp>
#include "SIGRunRegionResamplerTest.hpp"

namespace bp = boost::python;

TEST_F(SIGRunRegionResamplerTest, resample)
{
    char buf[FILENAME_MAX];
    getcwd(buf, FILENAME_MAX);
    std::string directory(buf);

    bp::import("sys").attr("path").attr("insert")(0, directory + "/tests/res/region");

    bp::object o = bp::import("Dummy1");
    o.attr(SI_INTERNAL_NAME) = "Dummy1";

    bp::object t = bp::import("Dummy2");
    t.attr(SI_INTERNAL_NAME) = "Dummy2";

    std::vector<glm::vec3> contour1 {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2 {glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1), glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, o, 1920, 1080);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, t, 1920, 1080);

    ASSERT_EQ(4, contour1.size());
    ASSERT_EQ(4, contour2.size());

    std::vector<glm::vec3> resample_a, resample_b;
    EXPECT_NO_FATAL_FAILURE(RegionResampler::resample(resample_a, contour1));
    EXPECT_NO_FATAL_FAILURE(RegionResampler::resample(resample_b, contour2));

    ASSERT_EQ(SI_CONTOUR_STEPCOUNT, resample_a.size());
    ASSERT_EQ(SI_CONTOUR_STEPCOUNT, resample_b.size());
}

TEST_F(SIGRunRegionResamplerTest, distance)
{
    std::vector<glm::vec3> contour1 {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

    ASSERT_NEAR(500.0f, rr_distance(contour1[0], contour1[1]), 1);
    ASSERT_NEAR(707.0f, rr_distance(contour1[0], contour1[2]), 1);
}

TEST_F(SIGRunRegionResamplerTest, total_length)
{
    std::vector<glm::vec3> contour1 {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

    ASSERT_NEAR(1500, rr_total_length(contour1), 1);
}