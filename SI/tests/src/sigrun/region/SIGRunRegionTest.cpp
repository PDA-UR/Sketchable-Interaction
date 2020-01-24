

#include <pysi/SuperEffect.hpp>
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

    bp::object o;

    EXPECT_NO_FATAL_FAILURE(Region r(contour, std::make_shared<bp::object>(o)));
}

TEST_F(SIGRunRegionTest, aabb)
{
    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

    bp::object o;

    Region r(contour, std::make_shared<bp::object>(o));

    ASSERT_EQ(contour.size(), r.aabb().size());

    for(int i = 0; i < contour.size(); ++i)
        ASSERT_EQ(contour[i], r.aabb()[i]);
}

TEST_F(SIGRunRegionTest, contour)
{
    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

    bp::object o;
    Region r(contour, std::make_shared<bp::object>(o));

    ASSERT_EQ(contour.size(), 4);
    ASSERT_EQ(r.contour().size(), 64);
}

TEST_F(SIGRunRegionTest, on_enter)
{
    std::string path = "res/region";

    std::vector<std::string> files, classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    std::string base_filename = files[0].substr(files[0].find_last_of("/\\") + 1);
    std::string module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    std::string rpath = path + "/" + base_filename;

    script.load_class_names(classes, files[0]);

    std::shared_ptr<bp::object> o = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    classes.clear();

    base_filename = files[1].substr(files[1].find_last_of("/\\") + 1);
    module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    rpath = path + "/" + base_filename;

    script.load_class_names(classes, files[1]);

    std::shared_ptr<bp::object> t = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

    Region r(contour, o);

    PySIEffect te = bp::extract<PySIEffect>(*t);

    ASSERT_FALSE(r.on_enter(te));
}

TEST_F(SIGRunRegionTest, on_continuous)
{
    std::string path = "res/region";

    std::vector<std::string> files, classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    std::string base_filename = files[0].substr(files[0].find_last_of("/\\") + 1);
    std::string module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    std::string rpath = path + "/" + base_filename;

    script.load_class_names(classes, files[0]);

    std::shared_ptr<bp::object> o = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    classes.clear();

    base_filename = files[1].substr(files[1].find_last_of("/\\") + 1);
    module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    rpath = path + "/" + base_filename;

    script.load_class_names(classes, files[1]);

    std::shared_ptr<bp::object> t = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

    Region r(contour, o);

    PySIEffect te = bp::extract<PySIEffect>(*t);

    ASSERT_FALSE(r.on_continuous(te));
}

TEST_F(SIGRunRegionTest, on_leave)
{
    std::string path = "res/region";

    std::vector<std::string> files, classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    std::string base_filename = files[0].substr(files[0].find_last_of("/\\") + 1);
    std::string module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    std::string rpath = path + "/" + base_filename;

    script.load_class_names(classes, files[0]);

    std::shared_ptr<bp::object> o = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    classes.clear();

    base_filename = files[1].substr(files[1].find_last_of("/\\") + 1);
    module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    rpath = path + "/" + base_filename;

    script.load_class_names(classes, files[1]);

    std::shared_ptr<bp::object> t = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

    Region r(contour, o);

    PySIEffect te = bp::extract<PySIEffect>(*t);

    ASSERT_FALSE(r.on_leave(te));
}