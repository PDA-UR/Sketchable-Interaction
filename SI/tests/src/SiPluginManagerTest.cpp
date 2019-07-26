

#include "include/SiPluginManagerTest.hpp"

TEST_F(SiPluginManagerTest, num_plugins)
{
    std::vector<bp::object> objects {bp::object()};
    EXPECT_NO_FATAL_FAILURE(add_plugins(objects));
    EXPECT_EQ(1, d_plugin_manager.num_plugins());
}

TEST_F(SiPluginManagerTest, plugins)
{
    std::vector<bp::object> objects {bp::object(), bp::object()};
    EXPECT_NO_FATAL_FAILURE(add_plugins(objects));

    EXPECT_NO_FATAL_FAILURE(d_plugin_manager.plugins());
    EXPECT_EQ(2, d_plugin_manager.plugins().size());
    EXPECT_EQ(2, d_plugin_manager.num_plugins());
}

TEST_F(SiPluginManagerTest, destroy)
{
    EXPECT_NO_FATAL_FAILURE(destroy());
}