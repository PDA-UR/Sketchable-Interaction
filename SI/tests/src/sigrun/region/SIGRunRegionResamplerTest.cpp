

#include <sigrun/plugin/PluginCollector.hpp>
#include <sigrun/plugin/Scripting.hpp>
#include <sigrun/region/Region.hpp>
#include "SIGRunRegionResamplerTest.hpp"

namespace bp = boost::python;

TEST_F(SIGRunRegionResamplerTest, resample)
{
    std::string path = "res/region";

    std::vector<std::string> files, classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    std::string base_filename = files[0].substr(files[0].find_last_of("/\\") + 1);
    std::string module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    std::string rpath = path + "/" + base_filename;

    script.load_class_names(classes, files[0]);

    bp::object o = script.si_plugin(module_name, rpath, classes[0]);

    classes.clear();

    base_filename = files[1].substr(files[1].find_last_of("/\\") + 1);
    module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    rpath = path + "/" + base_filename;

    script.load_class_names(classes, files[1]);

    bp::object t = script.si_plugin(module_name, rpath, classes[0]);

    std::vector<glm::vec3> contour1 {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2 {glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1), glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, o);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, t);

    ASSERT_EQ(4, a->contour().size());
    ASSERT_EQ(4, b->contour().size());

    std::vector<glm::vec3> resample_a, resample_b;
    EXPECT_NO_FATAL_FAILURE(RegionResampler::resample(resample_a, a->contour()));
    EXPECT_NO_FATAL_FAILURE(RegionResampler::resample(resample_b, b->contour()));

    ASSERT_EQ(64, resample_a.size());
    ASSERT_EQ(64, resample_b.size());
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