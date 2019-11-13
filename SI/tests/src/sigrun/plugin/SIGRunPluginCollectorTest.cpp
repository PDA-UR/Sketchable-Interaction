

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

//    char buf[FILENAME_MAX];
//    getcwd(buf, FILENAME_MAX);
//    std::string directory(buf);

//    ASSERT_EQ(directory + "/" + PATH + "/level0.py", files[0]);
//    ASSERT_EQ(directory + "/" + PATH + "/level1/level1.py", files[1]);
//    ASSERT_EQ(directory + "/" + PATH + "/level1/level2/level2.py", files[2]);
}

TEST_F(SIGRunPluginCollectorTest, meta_type)
{
    ASSERT_EQ(PluginCollector().meta_type(), "PLUGIN_COLLECTOR");
}