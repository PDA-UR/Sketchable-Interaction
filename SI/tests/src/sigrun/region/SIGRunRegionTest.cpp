

#include "SIGRunRegionTest.hpp"

#include <glm/glm.hpp>
#include <sigrun/region/Region.hpp>
#include <sigrun/region/RegionResampler.hpp>
#include <boost/python.hpp>
#include <sigrun/plugin/PluginCollector.hpp>
#include <sigrun/plugin/Scripting.hpp>
#include <debug/Print.hpp>

namespace bp = boost::python;

TEST_F(SIGRunRegionTest, construction)
{

    char buf[FILENAME_MAX];
    getcwd(buf, FILENAME_MAX);
    std::string directory(buf);

    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

    bp::import("sys").attr("path").attr("insert")(0, directory + "/tests/res/region");

    bp::object o = bp::import("Dummy3");
    o.attr(SI_INTERNAL_NAME) = "Dummy3";

    bp::dict kwargs;
    EXPECT_NO_FATAL_FAILURE(Region r(contour, o, 0, 0, kwargs));

    Region r(contour, o, 1920, 1080, kwargs);

    EXPECT_EQ(r.name(), "__ Dummy3 __");
}

TEST_F(SIGRunRegionTest, aabb)
{
    char buf[FILENAME_MAX];
    getcwd(buf, FILENAME_MAX);
    std::string directory(buf);

    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

//    bp::import("sys").attr("path").attr("insert")(0, directory + "/tests/res/region");

    bp::object o = bp::import("Dummy3");
    o.attr(SI_INTERNAL_NAME) = "Dummy3";
    bp::dict kwargs;

    Region r(contour, o, 1920, 1080, kwargs);

    ASSERT_EQ(contour.size(), r.aabb().size());
}

TEST_F(SIGRunRegionTest, contour)
{
    char buf[FILENAME_MAX];
    getcwd(buf, FILENAME_MAX);
    std::string directory(buf);

    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

//    bp::import("sys").attr("path").attr("insert")(0, directory + "/tests/res/region");

    bp::object o = bp::import("Dummy3");
    o.attr(SI_INTERNAL_NAME) = "Dummy3";
    bp::dict kwargs;
    Region r(contour, o, 1920, 1080, kwargs);

    ASSERT_EQ(contour.size(), 4);
    ASSERT_EQ(r.contour().size(), SI_CONTOUR_STEPCOUNT);
}

TEST_F(SIGRunRegionTest, on_enter)
{
    char buf[FILENAME_MAX];
    getcwd(buf, FILENAME_MAX);
    std::string directory(buf);

//    bp::import("sys").attr("path").attr("insert")(0, directory + "/tests/res/region");

    bp::object o = bp::import("Dummy1");
    o.attr(SI_INTERNAL_NAME) = "Dummy1";

    bp::object t = bp::import("Dummy2");
    t.attr(SI_INTERNAL_NAME) = "Dummy2";

    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};
    bp::dict kwargs;

    Region r(contour, o, 1920, 1080, kwargs);
    Region rt(contour, t, 1920, 1080, kwargs);

    EXPECT_NO_FATAL_FAILURE(r.on_enter(rt.effect()));
}

TEST_F(SIGRunRegionTest, on_continuous)
{
    char buf[FILENAME_MAX];
    getcwd(buf, FILENAME_MAX);
    std::string directory(buf);

//    bp::import("sys").attr("path").attr("insert")(0, directory + "/tests/res/region");

    bp::object o = bp::import("Dummy1");
    o.attr(SI_INTERNAL_NAME) = "Dummy1";

    bp::object t = bp::import("Dummy2");
    t.attr(SI_INTERNAL_NAME) = "Dummy2";

    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};
    bp::dict kwargs;

    Region r(contour, o, 1920, 1080, kwargs);
    Region rt(contour, t, 1920, 1080, kwargs);

    EXPECT_NO_FATAL_FAILURE(r.on_continuous(rt.effect()));
}

TEST_F(SIGRunRegionTest, on_leave)
{
    char buf[FILENAME_MAX];
    getcwd(buf, FILENAME_MAX);
    std::string directory(buf);

//    bp::import("sys").attr("path").attr("insert")(0, directory + "/tests/res/region");

    bp::object o = bp::import("Dummy1");
    o.attr(SI_INTERNAL_NAME) = "Dummy1";

    bp::object t = bp::import("Dummy2");
    t.attr(SI_INTERNAL_NAME) = "Dummy2";

    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};
    bp::dict kwargs;

    Region r(contour, o, 1920, 1080, kwargs);
    Region rt(contour, t, 1920, 1080, kwargs);

    EXPECT_NO_FATAL_FAILURE(r.on_leave(rt.effect()));
}