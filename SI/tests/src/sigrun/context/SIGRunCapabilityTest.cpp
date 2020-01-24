

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
    Capability c;

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

    EXPECT_NO_FATAL_FAILURE(c.add_capabilities(o));
    EXPECT_NO_FATAL_FAILURE(c.add_capabilities(t));

    ASSERT_EQ(c.num_capabilities(), 2);
    ASSERT_EQ(c.consecutive_capability_id(), 2);

    ASSERT_TRUE(c.capabilities().begin()->first == "TEST1");
    ASSERT_TRUE(c.capabilities().rbegin()->first == "TEST2");
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
