

#include "SIGRunScriptingTest.hpp"
#include <sigrun/plugin/Scripting.hpp>
#include <sigrun/plugin/PluginCollector.hpp>
#include <debug/Print.hpp>

TEST_F(SIGRunScriptingTest, construction)
{
    EXPECT_NO_FATAL_FAILURE(Scripting s);
}