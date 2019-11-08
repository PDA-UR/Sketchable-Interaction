
#include <debug/Print.hpp>
#include "SIGRunCoreTest.hpp"


TEST_F(SIGRunCoreTest, retrieve_available_plugins)
{
    std::unordered_map<std::string, std::shared_ptr<bp::object>> plugins;
    std::string path = "res/core/";

    core.retrieve_available_plugins(plugins, path);

    ASSERT_EQ(plugins.size(), 1);
    ASSERT_TRUE(plugins.begin()->first == "Test");
}