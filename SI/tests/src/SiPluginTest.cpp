#include <core/pysi_core/Scripting.hpp>
#include <core/pysi_core/PluginCollector.hpp>
#include <dirent.h>
#include "core/pysi_core/Plugin.hpp"
#include <core/debug/Print.hpp>
#include "include/SiPluginTest.hpp"

namespace bp = boost::python;


TEST_F(SIPluginTest, construction)
{
    EXPECT_NE(&d_py_object, nullptr);

    SIPlugin p;
    EXPECT_NO_FATAL_FAILURE(p.set_pyplugin(d_py_object));

    Print::print(p);
}

TEST_F(SIPluginTest, on_enter)
{
    SIPlugin p;
    EXPECT_NO_FATAL_FAILURE(p.set_pyplugin(d_py_object));

    EXPECT_NO_FATAL_FAILURE(p.on_enter());
    EXPECT_EQ(0, p.on_enter());
}

TEST_F(SIPluginTest, on_continuous)
{
    SIPlugin p;
    EXPECT_NO_FATAL_FAILURE(p.set_pyplugin(d_py_object));

    EXPECT_NO_FATAL_FAILURE(p.on_continuous());
    EXPECT_EQ(0, p.on_continuous());
}

TEST_F(SIPluginTest, on_leave)
{
    SIPlugin p;
    EXPECT_NO_FATAL_FAILURE(p.set_pyplugin(d_py_object));

    EXPECT_NO_FATAL_FAILURE(p.on_leave());
    EXPECT_EQ(0, p.on_leave());
}

TEST_F(SIPluginTest, print)
{
    SIPlugin p;
    EXPECT_NO_FATAL_FAILURE(p.set_pyplugin(d_py_object));

    EXPECT_NO_FATAL_FAILURE(Print::print(p));
    EXPECT_NO_FATAL_FAILURE(std::cout << p << std::endl);
}