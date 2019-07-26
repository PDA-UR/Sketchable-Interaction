

#ifndef SI_SIPLUGINMANAGERTEST_HPP
#define SI_SIPLUGINMANAGERTEST_HPP


#include <gtest/gtest.h>
#include <context/managers/PluginManager.hpp>

class SiPluginManagerTest : public ::testing::Test
{
public:
    virtual void SetUp()
    {

    }

    virtual void TearDown()
    {

    }

    PluginManager d_plugin_manager;

protected:
    void add_plugins(const std::vector<bp::object>& objects)
    {
        d_plugin_manager.add_plugins(objects);
    }

    void destroy()
    {
        d_plugin_manager.destroy();
    }
};


#endif //SI_SIPLUGINMANAGERTEST_HPP
