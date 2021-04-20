

#include <sigrun/plugin/PluginCollector.hpp>
#include <sigrun/plugin/Scripting.hpp>
#include <sigrun/region/Region.hpp>
#include "SIGRunRegionResamplerTest.hpp"

namespace bp = boost::python;

TEST_F(SIGRunRegionResamplerTest, resample)
{
    std::string path = "tests/res/region";

    std::vector<std::tuple<std::string, std::string>> files;
    std::vector<std::string> classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    const std::string& full_path = std::get<0>(files[0]);
    const std::string& name = std::get<1>(files[0]);

    std::string module_name = name.substr(0, name.find_last_of('.'));
    std::string rpath = full_path.substr(full_path.find(path)) + "/" + name;

    bp::object o = script.si_plugin(module_name, rpath);

    classes.clear();

    const std::string& full_path2 = std::get<0>(files[1]);
    const std::string& name2 = std::get<1>(files[1]);

    module_name = name2.substr(0, name2.find_last_of('.'));
    rpath = full_path2.substr(full_path2.find(path)) + "/" + name2;

    std::shared_ptr<bp::object> t = std::make_shared<bp::object>(script.si_plugin(module_name, rpath));

    std::vector<glm::vec3> contour1 {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2 {glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1), glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, *o, 1920, 1080);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, *t, 1920, 1080);

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