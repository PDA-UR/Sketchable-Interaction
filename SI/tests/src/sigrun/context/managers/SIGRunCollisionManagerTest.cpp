

#include "SIGRunCollisionManagerTest.hpp"
#include <sigrun/region/Region.hpp>
#include <sigrun/plugin/Scripting.hpp>
#include <sigrun/plugin/PluginCollector.hpp>
#include <unordered_map>
#include <boost/python.hpp>
#include <memory>

namespace bp = boost::python;

TEST_F(SIGRunCollisionManagerTest, collide)
{
    char buf[FILENAME_MAX];
    getcwd(buf, FILENAME_MAX);
    std::string directory(buf);

    bp::import("sys").attr("path").attr("insert")(0, directory + "/tests/res/region");

    bp::object o = bp::import("Dummy1");
    o.attr(SI_INTERNAL_NAME) = "Dummy1";

    bp::object t = bp::import("Dummy2");
    t.attr(SI_INTERNAL_NAME) = "Dummy2";

    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

    std::shared_ptr<Region> r = std::make_shared<Region>(contour, o, 1920, 1080);
    std::shared_ptr<Region> s = std::make_shared<Region>(contour, t, 1920, 1080);

    std::vector<std::shared_ptr<Region>> v = {r, s};

    EXPECT_NO_FATAL_FAILURE(cm.collide(v));
}

TEST_F(SIGRunCollisionManagerTest, has_capabilities_in_common)
{
    char buf[FILENAME_MAX];
    getcwd(buf, FILENAME_MAX);
    std::string directory(buf);

    bp::import("sys").attr("path").attr("insert")(0, directory + "/tests/res/region");

    bp::object o = bp::import("Dummy1");
    o.attr(SI_INTERNAL_NAME) = "Dummy1";

    bp::object t = bp::import("Dummy2");
    t.attr(SI_INTERNAL_NAME) = "Dummy2";

    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

    std::shared_ptr<Region> r = std::make_shared<Region>(contour, o, 1920, 1080);
    std::shared_ptr<Region> s = std::make_shared<Region>(contour, t, 1920, 1080);

    std::vector<std::shared_ptr<Region>> v = {r, s};

    ASSERT_TRUE(cm_has_capabilities_in_common(r, s));
}

TEST_F(SIGRunCollisionManagerTest, collides_with_aabb)
{
    char buf[FILENAME_MAX];
    getcwd(buf, FILENAME_MAX);
    std::string directory(buf);

    bp::import("sys").attr("path").attr("insert")(0, directory + "/tests/res/region");

    bp::object o = bp::import("Dummy1");
    o.attr(SI_INTERNAL_NAME) = "Dummy1";

    bp::object t = bp::import("Dummy2");
    t.attr(SI_INTERNAL_NAME) = "Dummy2";

    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour, o, 1920, 1080);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour, t, 1920, 1080);

    ASSERT_TRUE(cm_collides_with_aabb(a, b));
}

TEST_F(SIGRunCollisionManagerTest, cm_collides_with_mask)
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

    ASSERT_TRUE(cm_collides_with_aabb(a, b));
    ASSERT_TRUE(cm_collides_with_mask(a, b));
    ASSERT_TRUE(cm_collides_with_mask(b, a));
}

TEST_F(SIGRunCollisionManagerTest, handle_event_continuous)
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

    EXPECT_NO_FATAL_FAILURE(cm_handle_event_continuous(a, b));
}

TEST_F(SIGRunCollisionManagerTest, handle_event_enter)
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

    auto tuple = std::make_tuple(a->uuid(), b->uuid());

    EXPECT_NO_FATAL_FAILURE(cm_handle_event_enter(a, b));
}

TEST_F(SIGRunCollisionManagerTest, handle_event_leave)
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

    auto tuple = std::make_tuple(a->uuid(), b->uuid());

    EXPECT_NO_FATAL_FAILURE(cm_handle_event_leave(a, b));
}