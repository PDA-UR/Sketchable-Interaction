

#include "include/SiScriptingTest.hpp"

#include <core/pysi_core/Scripting.hpp>
#include "core/pysi_core/Plugin.hpp"
#include <core/pysi_core/PluginCollector.hpp>
#include <core/debug/Print.hpp>

namespace bp = boost::python;

TEST_F(SIScriptingTest, construction)
{
    EXPECT_NE(&d_script, nullptr);
}

TEST_F(SIScriptingTest, load_plugin_source)
{
    const std::string rel_path = "tests/res";
    std::vector<std::string> files, classes;

    PluginCollector::collect("/" + rel_path, files);

    std::string s = d_script.load_plugin_source(files[0].c_str());

    EXPECT_TRUE(s.size());
}

TEST_F(SIScriptingTest, load_class_names)
{
    const std::string rel_path = "tests/res";
    std::vector<std::string> files, classes;

    PluginCollector::collect("/" + rel_path, files);

    std::string base_filename = files[0].substr(files[0].find_last_of("/\\") + 1);
    std::string module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    std::string rpath = rel_path + "/" + base_filename;

    d_script.load_class_names(classes, files[0]);

    EXPECT_TRUE(classes.size());
    EXPECT_TRUE(classes[0] == "PyRegion");
}

TEST_F(SIScriptingTest, import)
{
    const std::string rel_path = "tests/res";
    std::vector<std::string> files, classes;

    PluginCollector::collect("/" + rel_path, files);

    std::string base_filename = files[0].substr(files[0].find_last_of("/\\") + 1);
    std::string module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    std::string rpath = rel_path + "/" + base_filename;

    d_script.load_class_names(classes, files[0]);

    EXPECT_NO_FATAL_FAILURE(d_script.import(module_name, rpath));

    auto x = d_script.import(module_name, rpath);
    ASSERT_TRUE(&x);

    EXPECT_NO_FATAL_FAILURE(x.attr(classes[0].c_str()));

    auto d = x.attr(classes[0].c_str());
    ASSERT_TRUE(&d);
}

TEST_F(SIScriptingTest, print)
{
    EXPECT_NO_FATAL_FAILURE(Print::print(d_script));
    EXPECT_NO_FATAL_FAILURE(std::cout << d_script << std::endl);
}