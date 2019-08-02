

#include <core/pysi_core/PluginCollector.hpp>
#include "include/SiPluginCollectorTest.hpp"

TEST_F(SiPluginCollectorTest, collect)
{
    std::vector<std::string> files;

    EXPECT_FALSE(files.size());
    EXPECT_NO_FATAL_FAILURE(PluginCollector::collect("/" + PATH, files));
    EXPECT_TRUE(files.size());
    EXPECT_EQ(1, files.size());

    char buf[FILENAME_MAX];
    getcwd(buf, FILENAME_MAX);
    std::string directory(buf);

    ASSERT_EQ(directory + "/" + PATH + "/test.py", files[0]);
}