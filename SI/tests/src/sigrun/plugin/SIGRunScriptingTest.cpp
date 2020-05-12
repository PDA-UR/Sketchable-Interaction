

#include "SIGRunScriptingTest.hpp"
#include <sigrun/plugin/Scripting.hpp>
#include <sigrun/plugin/PluginCollector.hpp>
#include <debug/Print.hpp>

TEST_F(SIGRunScriptingTest, construction)
{
    EXPECT_NO_FATAL_FAILURE(Scripting s);
}

TEST_F(SIGRunScriptingTest, load_plugin_source)
{
    Scripting script;

    const std::string rel_path = "tests/res/scripting";
    std::vector<std::tuple<std::string, std::string>> files;
    std::vector<std::string> classes;

    PluginCollector().collect("/" + rel_path, files);

    std::string s = script.load_plugin_source((std::get<0>(files[0]) + "/" + std::get<1>(files[0])).c_str());

    EXPECT_TRUE(s.size());
}

TEST_F(SIGRunScriptingTest, load_class_names)
{
    Scripting script;

    const std::string rel_path = "tests/res/scripting";
    std::vector<std::tuple<std::string, std::string>> files;
    std::vector<std::string> classes;

    PluginCollector().collect("/" + rel_path, files);

    const std::string& full_path = std::get<0>(files[0]);
    const std::string& name = std::get<1>(files[0]);

    std::string module_name = name.substr(0, name.find_last_of('.'));
    std::string rpath = full_path.substr(full_path.find(rel_path)) + "/" + name;

    script.load_class_names(classes, rpath);

    EXPECT_TRUE(classes.size());
    EXPECT_TRUE(classes[0] == "Test");
}
