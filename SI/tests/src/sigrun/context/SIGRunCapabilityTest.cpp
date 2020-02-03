

#include "SIGRunCapabilityTest.hpp"

#include <sigrun/context/Capability.hpp>
#include <sigrun/plugin/Scripting.hpp>
#include <sigrun/plugin/PluginCollector.hpp>

TEST_F(SIGRunCapabilityTest, construction)
{
    EXPECT_NO_FATAL_FAILURE(Capability c);
}

TEST_F(SIGRunCapabilityTest, add_capabilities)
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
    rpath = full_path2.substr(full_path2.find(path)) + "/" + name;

    script.load_class_names(classes, rpath);

    bp::object t = script.si_plugin(module_name, rpath, classes[0]);

    auto c = std::make_unique<Capability>();

    c->add_capabilities(o);
    c->add_capabilities(t);

    ASSERT_EQ(c->num_capabilities(), 2);
    ASSERT_EQ(c->consecutive_capability_id(), 2);

    ASSERT_TRUE(c->capabilities().begin()->first == "TEST1");
    ASSERT_TRUE(c->capabilities().rbegin()->first == "TEST2");
}

TEST_F(SIGRunCapabilityTest, add_capabilitiy)
{
    Capability c;

    EXPECT_NO_FATAL_FAILURE(c.add_capability("TEST"));
    ASSERT_TRUE(c.num_capabilities() == 1);
    ASSERT_TRUE(c.consecutive_capability_id() == 1);

    ASSERT_TRUE(c.capabilities().begin()->first == "TEST");
}

TEST_F(SIGRunCapabilityTest, remove_capability)
{
    Capability c;

    EXPECT_NO_FATAL_FAILURE(c.add_capability("TEST"));
    ASSERT_TRUE(c.num_capabilities() == 1);
    ASSERT_TRUE(c.consecutive_capability_id() == 1);

    ASSERT_TRUE(c.capabilities().begin()->first == "TEST");
    EXPECT_NO_FATAL_FAILURE(c.remove_capability("TEST"));

    ASSERT_TRUE(c.num_capabilities() == 0);
    ASSERT_TRUE(c.consecutive_capability_id() == 1);

    ASSERT_TRUE(c.capabilities().empty());
}
