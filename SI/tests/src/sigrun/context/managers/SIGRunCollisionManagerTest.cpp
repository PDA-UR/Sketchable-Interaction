

#include "SIGRunCollisionManagerTest.hpp"
#include <sigrun/region/Region.hpp>
#include <sigrun/plugin/Scripting.hpp>
#include <sigrun/plugin/PluginCollector.hpp>
#include <sigrun/context/Context.hpp>
#include <unordered_map>
#include <boost/python.hpp>
#include <memory>

namespace bp = boost::python;

TEST_F(SIGRunCollisionManagerTest, collide)
{
    std::string path = "res/region";

    std::vector<std::tuple<std::string, std::string>> files;
    std::vector<std::string> classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    const std::string& full_path = std::get<0>(files[0]);
    const std::string& name = std::get<1>(files[0]);

    std::string module_name = name.substr(0, name.find_last_of('.'));
    std::string rpath = full_path.substr(full_path.find(path)) + "/" + name;

    script.load_class_names(classes, rpath);

    bp::object o = script.si_plugin(module_name, rpath, classes[0]);

    classes.clear();

    const std::string& full_path2 = std::get<0>(files[1]);
    const std::string& name2 = std::get<1>(files[1]);

    module_name = name2.substr(0, name2.find_last_of('.'));
    rpath = full_path2.substr(full_path2.find(path)) + "/" + name2;

    script.load_class_names(classes, rpath);

    std::shared_ptr<bp::object> t = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

    std::shared_ptr<Region> r = std::make_shared<Region>(contour, *o);
    std::shared_ptr<Region> s = std::make_shared<Region>(contour, *t);

    std::vector<std::shared_ptr<Region>> v = {r, s};

    EXPECT_NO_FATAL_FAILURE(cm.collide(v));
}

TEST_F(SIGRunCollisionManagerTest, has_capabilities_in_common)
{
    std::string path = "res/region";

    std::vector<std::tuple<std::string, std::string>> files;
    std::vector<std::string> classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    const std::string& full_path = std::get<0>(files[0]);
    const std::string& name = std::get<1>(files[0]);

    std::string module_name = name.substr(0, name.find_last_of('.'));
    std::string rpath = full_path.substr(full_path.find(path)) + "/" + name;

    script.load_class_names(classes, rpath);

    bp::object o = script.si_plugin(module_name, rpath, classes[0]);

    classes.clear();

    const std::string& full_path2 = std::get<0>(files[1]);
    const std::string& name2 = std::get<1>(files[1]);

    module_name = name2.substr(0, name2.find_last_of('.'));
    rpath = full_path2.substr(full_path2.find(path)) + "/" + name2;

    script.load_class_names(classes, rpath);

    std::shared_ptr<bp::object> t = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

    std::shared_ptr<Region> r = std::make_shared<Region>(contour, *o);
    std::shared_ptr<Region> s = std::make_shared<Region>(contour, *t);

    ASSERT_TRUE(cm_has_capabilities_in_common(r, s));
}

TEST_F(SIGRunCollisionManagerTest, collides_with_aabb)
{
    std::string path = "res/region";

    std::vector<std::tuple<std::string, std::string>> files;
    std::vector<std::string> classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    const std::string& full_path = std::get<0>(files[0]);
    const std::string& name = std::get<1>(files[0]);

    std::string module_name = name.substr(0, name.find_last_of('.'));
    std::string rpath = full_path.substr(full_path.find(path)) + "/" + name;

    script.load_class_names(classes, rpath);

    bp::object o = script.si_plugin(module_name, rpath, classes[0]);

    classes.clear();

    const std::string& full_path2 = std::get<0>(files[1]);
    const std::string& name2 = std::get<1>(files[1]);

    module_name = name2.substr(0, name2.find_last_of('.'));
    rpath = full_path2.substr(full_path2.find(path)) + "/" + name2;

    script.load_class_names(classes, rpath);

    std::shared_ptr<bp::object> t = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour, *o);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour, *t);

    ASSERT_TRUE(cm_collides_with_aabb(a, b));
}

TEST_F(SIGRunCollisionManagerTest, is_aabb_enveloped_negative)
{
    std::string path = "res/region";

    std::vector<std::tuple<std::string, std::string>> files;
    std::vector<std::string> classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    const std::string& full_path = std::get<0>(files[0]);
    const std::string& name = std::get<1>(files[0]);

    std::string module_name = name.substr(0, name.find_last_of('.'));
    std::string rpath = full_path.substr(full_path.find(path)) + "/" + name;

    script.load_class_names(classes, rpath);

    bp::object o = script.si_plugin(module_name, rpath, classes[0]);

    classes.clear();

    const std::string& full_path2 = std::get<0>(files[1]);
    const std::string& name2 = std::get<1>(files[1]);

    module_name = name2.substr(0, name2.find_last_of('.'));
    rpath = full_path2.substr(full_path2.find(path)) + "/" + name2;

    script.load_class_names(classes, rpath);

    std::shared_ptr<bp::object> t = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour, *o);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour, *t);

    ASSERT_FALSE(cm_is_aabb_enveloped(a, b));
    ASSERT_TRUE(cm_collides_with_aabb(a, b));
}

TEST_F(SIGRunCollisionManagerTest, is_aabb_enveloped_positive)
{
    std::string path = "res/region";

    std::vector<std::tuple<std::string, std::string>> files;
    std::vector<std::string> classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    const std::string& full_path = std::get<0>(files[0]);
    const std::string& name = std::get<1>(files[0]);

    std::string module_name = name.substr(0, name.find_last_of('.'));
    std::string rpath = full_path.substr(full_path.find(path)) + "/" + name;

    script.load_class_names(classes, rpath);

    bp::object o = script.si_plugin(module_name, rpath, classes[0]);

    classes.clear();

    const std::string& full_path2 = std::get<0>(files[1]);
    const std::string& name2 = std::get<1>(files[1]);

    module_name = name2.substr(0, name2.find_last_of('.'));
    rpath = full_path2.substr(full_path2.find(path)) + "/" + name2;

    script.load_class_names(classes, rpath);

    std::shared_ptr<bp::object> t = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    std::vector<glm::vec3> contour1 {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2 {glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1), glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, *o);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, *t);

    ASSERT_TRUE(cm_is_aabb_enveloped(a, b));
    ASSERT_TRUE(cm_collides_with_aabb(a, b));
}

TEST_F(SIGRunCollisionManagerTest, cm_collides_with_mask)
{
    std::string path = "res/region";

    std::vector<std::tuple<std::string, std::string>> files;
    std::vector<std::string> classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    const std::string& full_path = std::get<0>(files[0]);
    const std::string& name = std::get<1>(files[0]);

    std::string module_name = name.substr(0, name.find_last_of('.'));
    std::string rpath = full_path.substr(full_path.find(path)) + "/" + name;

    script.load_class_names(classes, rpath);

    bp::object o = script.si_plugin(module_name, rpath, classes[0]);

    classes.clear();

    const std::string& full_path2 = std::get<0>(files[1]);
    const std::string& name2 = std::get<1>(files[1]);

    module_name = name2.substr(0, name2.find_last_of('.'));
    rpath = full_path2.substr(full_path2.find(path)) + "/" + name2;

    script.load_class_names(classes, rpath);

    std::shared_ptr<bp::object> t = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    std::vector<glm::vec3> contour1 {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2 {glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1), glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, *o);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, *t);
    std::shared_ptr<Region> c = std::make_shared<Region>(contour1, *t);

    ASSERT_TRUE(cm_is_aabb_enveloped(a, b));
    ASSERT_TRUE(cm_collides_with_aabb(a, b));
    ASSERT_TRUE(cm_collides_with_mask(a, b));
    ASSERT_TRUE(cm_collides_with_mask(b, a));
}

TEST_F(SIGRunCollisionManagerTest, are_aabbs_equal)
{
    std::string path = "res/region";

    std::vector<std::tuple<std::string, std::string>> files;
    std::vector<std::string> classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    const std::string& full_path = std::get<0>(files[0]);
    const std::string& name = std::get<1>(files[0]);

    std::string module_name = name.substr(0, name.find_last_of('.'));
    std::string rpath = full_path.substr(full_path.find(path)) + "/" + name;

    script.load_class_names(classes, rpath);

    bp::object o = script.si_plugin(module_name, rpath, classes[0]);

    classes.clear();

    const std::string& full_path2 = std::get<0>(files[1]);
    const std::string& name2 = std::get<1>(files[1]);

    module_name = name2.substr(0, name2.find_last_of('.'));
    rpath = full_path2.substr(full_path2.find(path)) + "/" + name2;

    script.load_class_names(classes, rpath);

    std::shared_ptr<bp::object> t = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    std::vector<glm::vec3> contour1 {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2 {glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1), glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, *o);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, *t);
    std::shared_ptr<Region> c = std::make_shared<Region>(contour1, *t);

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

    std::vector<std::tuple<std::string, std::string>> files;
    std::vector<std::string> classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    const std::string& full_path = std::get<0>(files[0]);
    const std::string& name = std::get<1>(files[0]);

    std::string module_name = name.substr(0, name.find_last_of('.'));
    std::string rpath = full_path.substr(full_path.find(path)) + "/" + name;

    script.load_class_names(classes, rpath);

    bp::object o = script.si_plugin(module_name, rpath, classes[0]);

    classes.clear();

    const std::string& full_path2 = std::get<0>(files[1]);
    const std::string& name2 = std::get<1>(files[1]);

    module_name = name2.substr(0, name2.find_last_of('.'));
    rpath = full_path2.substr(full_path2.find(path)) + "/" + name2;

    script.load_class_names(classes, rpath);

    std::shared_ptr<bp::object> t = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    std::vector<glm::vec3> contour1 {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2 {glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1), glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, *o);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, *t);

    EXPECT_NO_FATAL_FAILURE(cm_handle_event_continuous(a, b));
}

TEST_F(SIGRunCollisionManagerTest, handle_event_enter)
{
    std::string path = "res/region";

    std::vector<std::tuple<std::string, std::string>> files;
    std::vector<std::string> classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    const std::string& full_path = std::get<0>(files[0]);
    const std::string& name = std::get<1>(files[0]);

    std::string module_name = name.substr(0, name.find_last_of('.'));
    std::string rpath = full_path.substr(full_path.find(path)) + "/" + name;

    script.load_class_names(classes, rpath);

    bp::object o = script.si_plugin(module_name, rpath, classes[0]);

    classes.clear();

    const std::string& full_path2 = std::get<0>(files[1]);
    const std::string& name2 = std::get<1>(files[1]);

    module_name = name2.substr(0, name2.find_last_of('.'));
    rpath = full_path2.substr(full_path2.find(path)) + "/" + name2;

    script.load_class_names(classes, rpath);

    std::shared_ptr<bp::object> t = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    std::vector<glm::vec3> contour1 {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2 {glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1), glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, *o);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, *t);

    auto tuple = std::make_tuple(a->uuid(), b->uuid());

    EXPECT_NO_FATAL_FAILURE(cm_handle_event_enter(a, b, tuple));
}

TEST_F(SIGRunCollisionManagerTest, handle_event_leave)
{
    std::string path = "res/region";

    std::vector<std::tuple<std::string, std::string>> files;
    std::vector<std::string> classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    const std::string& full_path = std::get<0>(files[0]);
    const std::string& name = std::get<1>(files[0]);

    std::string module_name = name.substr(0, name.find_last_of('.'));
    std::string rpath = full_path.substr(full_path.find(path)) + "/" + name;

    script.load_class_names(classes, rpath);

    bp::object o = script.si_plugin(module_name, rpath, classes[0]);

    classes.clear();

    const std::string& full_path2 = std::get<0>(files[1]);
    const std::string& name2 = std::get<1>(files[1]);

    module_name = name2.substr(0, name2.find_last_of('.'));
    rpath = full_path2.substr(full_path2.find(path)) + "/" + name2;

    script.load_class_names(classes, rpath);

    std::shared_ptr<bp::object> t = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    std::vector<glm::vec3> contour1 {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2 {glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1), glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, *o);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, *t);

    auto tuple = std::make_tuple(a->uuid(), b->uuid());

    EXPECT_NO_FATAL_FAILURE(cm_handle_event_leave(a, b, tuple));
}