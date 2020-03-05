

#include "SIGRunRegionTest.hpp"

#include <glm/glm.hpp>
#include <sigrun/region/Region.hpp>
#include <boost/python.hpp>
#include <sigrun/plugin/PluginCollector.hpp>
#include <sigrun/plugin/Scripting.hpp>

namespace bp = boost::python;

TEST_F(SIGRunRegionTest, construction)
{
    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

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

    std::shared_ptr<bp::object> o = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    EXPECT_NO_FATAL_FAILURE(Region r(contour, *o, 1920, 1080));
}

TEST_F(SIGRunRegionTest, aabb)
{
    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

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

    std::shared_ptr<bp::object> o = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    Region r(contour, *o, 1920, 1080);

    ASSERT_EQ(contour.size(), r.aabb().size());

    for(int i = 0; i < contour.size(); ++i)
        ASSERT_EQ(contour[i], r.aabb()[i]);
}

TEST_F(SIGRunRegionTest, contour)
{
    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

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

    std::shared_ptr<bp::object> o = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));
    Region r(contour, *o, 1920, 1080);

    ASSERT_EQ(contour.size(), 4);
    ASSERT_EQ(r.contour().size(), 64);
}

TEST_F(SIGRunRegionTest, on_enter)
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

    Region r(contour, *o, 1920, 1080);

    PySIEffect te = bp::extract<PySIEffect>(*t);

    EXPECT_NO_FATAL_FAILURE(r.on_enter(te));
}

TEST_F(SIGRunRegionTest, on_continuous)
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

    Region r(contour, *o, 1920, 1080);

    PySIEffect te = bp::extract<PySIEffect>(*t);

    EXPECT_NO_FATAL_FAILURE(r.on_continuous(te));
}

TEST_F(SIGRunRegionTest, on_leave)
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

    Region r(contour, *o, 1920, 1080);

    PySIEffect te = bp::extract<PySIEffect>(*t);

    EXPECT_NO_FATAL_FAILURE(r.on_leave(te));
}