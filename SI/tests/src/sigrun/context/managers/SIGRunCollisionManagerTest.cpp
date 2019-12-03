

#include "SIGRunCollisionManagerTest.hpp"
#include <sigrun/region/Region.hpp>
#include <sigrun/plugin/Scripting.hpp>
#include <sigrun/plugin/PluginCollector.hpp>

TEST_F(SIGRunCollisionManagerTest, collide)
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

    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

    std::shared_ptr<Region> r = std::make_shared<Region>(contour, o);
    std::shared_ptr<Region> s = std::make_shared<Region>(contour, t);

    std::vector<std::shared_ptr<Region>> v = {r, s};

    EXPECT_NO_FATAL_FAILURE(cm.collide(v));
}

TEST_F(SIGRunCollisionManagerTest, has_capabilities_in_common)
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

    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

    std::shared_ptr<Region> r = std::make_shared<Region>(contour, o);
    std::shared_ptr<Region> s = std::make_shared<Region>(contour, t);

    ASSERT_TRUE(cm_has_capabilities_in_common(r, s));
}

TEST_F(SIGRunCollisionManagerTest, collides_with_aabb)
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

    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour, o);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour, t);

    ASSERT_TRUE(cm_collides_with_aabb(a, b));
}

TEST_F(SIGRunCollisionManagerTest, is_aabb_enveloped_negative)
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

    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour, o);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour, t);

    ASSERT_FALSE(cm_is_aabb_enveloped(a, b));
    ASSERT_TRUE(cm_collides_with_aabb(a, b));
}

TEST_F(SIGRunCollisionManagerTest, is_aabb_enveloped_positive)
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

    ASSERT_TRUE(cm_is_aabb_enveloped(a, b));
    ASSERT_TRUE(cm_collides_with_aabb(a, b));
}

TEST_F(SIGRunCollisionManagerTest, cm_collides_with_mask)
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
    std::shared_ptr<Region> c = std::make_shared<Region>(contour1, t);

    ASSERT_TRUE(cm_is_aabb_enveloped(a, b));
    ASSERT_TRUE(cm_collides_with_aabb(a, b));
    ASSERT_TRUE(cm_collides_with_mask(a, b));
    ASSERT_TRUE(cm_collides_with_mask(b, a));
}

TEST_F(SIGRunCollisionManagerTest, are_aabbs_equal)
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
    std::shared_ptr<Region> c = std::make_shared<Region>(contour1, t);

    ASSERT_TRUE(cm_is_aabb_enveloped(a, b));
    ASSERT_TRUE(cm_collides_with_aabb(a, b));
    ASSERT_TRUE(cm_collides_with_mask(a, b));
    ASSERT_TRUE(cm_collides_with_mask(b, a));

    ASSERT_TRUE(cm_are_aabbs_equal(a, c));
    ASSERT_TRUE(cm_are_aabbs_equal(c, a));
    ASSERT_FALSE(cm_are_aabbs_equal(a, b));
    ASSERT_FALSE(cm_are_aabbs_equal(b, a));
    ASSERT_FALSE(cm_are_aabbs_equal(b, c));
    ASSERT_FALSE(cm_are_aabbs_equal(c, b));
}

TEST_F(SIGRunCollisionManagerTest, handle_event_continuous)
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

    EXPECT_NO_FATAL_FAILURE(cm_handle_event_continuous(a, b));
}

TEST_F(SIGRunCollisionManagerTest, handle_event_enter)
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

    auto tuple = std::make_tuple(a->uuid(), b->uuid());

    EXPECT_NO_FATAL_FAILURE(cm_handle_event_enter(a, b, tuple));


}

TEST_F(SIGRunCollisionManagerTest, handle_event_leave)
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

    auto tuple = std::make_tuple(a->uuid(), b->uuid());

    EXPECT_NO_FATAL_FAILURE(cm_handle_event_leave(a, b, tuple));
}