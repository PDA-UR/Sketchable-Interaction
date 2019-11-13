

#include "SIGRunPluginCollectorTest.hpp"

#include <sigrun/plugin/PluginCollector.hpp>
#include <debug/Print.hpp>

TEST_F(SIGRunPluginCollectorTest, construction)
{
    EXPECT_NO_FATAL_FAILURE(PluginCollector pc);
}

TEST_F(SIGRunPluginCollectorTest, collect)
{
    std::vector<std::string> files;

    EXPECT_FALSE(files.size());
    EXPECT_NO_FATAL_FAILURE(PluginCollector().collect("/" + PATH, files));
    EXPECT_TRUE(files.size());
    EXPECT_EQ(3, files.size());

    char buf[FILENAME_MAX];
    getcwd(buf, FILENAME_MAX);
    std::string directory(buf);
    directory += "/" + PATH;

    std::string level0 = directory + "/level0.py";
    std::string level1 = directory + "/level1/level1.py";
    std::string level2 = directory + "/level1/level2/level2.py";

    bool found_l0 = false, found_l1 = false, found_l2 = false;

    for(auto& file : files)
    {
        if (file == level0)
            found_l0 = true;
        if (file == level1)
            found_l1 = true;
        if (file == level2)
            found_l2 = true;
    }

    EXPECT_TRUE(found_l0 && found_l1 && found_l2);
}

TEST_F(SIGRunPluginCollectorTest, meta_type)
{
    ASSERT_EQ(PluginCollector().meta_type(), "PLUGIN_COLLECTOR");
}