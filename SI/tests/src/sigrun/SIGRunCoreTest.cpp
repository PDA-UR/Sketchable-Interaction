
#include <debug/Print.hpp>
#include "SIGRunCoreTest.hpp"

TEST_F(SIGRunCoreTest, retrieve_available_plugins)
{
    std::unordered_map<std::string, std::unique_ptr<bp::object>> plugins;
    std::string path = "tests/res/core";

    core_retrieve_available_plugins(plugins, path);

    ASSERT_EQ(plugins.size(), 1);
    ASSERT_EQ(plugins.begin()->first, "TESTREGION");
}