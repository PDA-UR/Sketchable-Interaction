

#include "SIGRunScriptingTest.hpp"
#include <sigrun/plugin/Scripting.hpp>
#include <sigrun/plugin/PluginCollector.hpp>
#include <debug/Print.hpp>

TEST_F(SIGRunScriptingTest, construnction)
{
    EXPECT_NO_FATAL_FAILURE(Scripting s);
}

TEST_F(SIGRunScriptingTest, load_plugin_source)
{
    Scripting script;

    const std::string rel_path = "res/scripting";
    std::vector<std::string> files, classes;

    PluginCollector().collect("/" + rel_path, files);

    Print::print(files.size());

    std::string s = script.load_plugin_source(files[0].c_str());

    EXPECT_TRUE(s.size());
}

TEST_F(SIGRunScriptingTest, load_class_names)
{
    Scripting script;

    const std::string rel_path = "res/scripting";
    std::vector<std::string> files, classes;

    PluginCollector().collect("/" + rel_path, files);

    std::string base_filename = files[0].substr(files[0].find_last_of("/\\") + 1);
    std::string module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    std::string rpath = rel_path + "/" + base_filename;

    script.load_class_names(classes, files[0]);

    EXPECT_TRUE(classes.size());
    EXPECT_TRUE(classes[0] == "Test");
}

TEST_F(SIGRunScriptingTest, import)
{
    Scripting script;

    const std::string rel_path = "res/scripting";
    std::vector<std::string> files, classes;

    PluginCollector().collect("/" + rel_path, files);

    std::string base_filename = files[0].substr(files[0].find_last_of("/\\") + 1);
    std::string module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    std::string rpath = rel_path + "/" + base_filename;

    script.load_class_names(classes, files[0]);

    EXPECT_NO_FATAL_FAILURE(script.import(module_name, rpath));

    auto x = script.import(module_name, rpath);
    ASSERT_TRUE(&x);

    EXPECT_NO_FATAL_FAILURE(x.attr(classes[0].c_str()));

    auto d = x.attr(classes[0].c_str());
    ASSERT_TRUE(&d);
}