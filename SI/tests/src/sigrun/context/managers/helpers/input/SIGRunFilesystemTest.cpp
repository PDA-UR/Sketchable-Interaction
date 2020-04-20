

#include "SIGRunFilesystemTest.hpp"

#include <sigrun/context/managers/helpers/input/FileSystem.hpp>

// difficult to test in a filesystem agnostic and generic manner

TEST_F(SIGRunFilesystemTest, construction)
{
    ASSERT_NO_FATAL_FAILURE(FileSystem fs);
}

TEST_F(SIGRunFilesystemTest, set_cwd)
{
    FileSystem fs;
    const std::string path = "";
    ASSERT_NO_FATAL_FAILURE(fs.set_cwd(path));
}

TEST_F(SIGRunFilesystemTest, cwd)
{
    FileSystem fs;

    ASSERT_NO_FATAL_FAILURE(fs.cwd());
    ASSERT_NO_FATAL_FAILURE(const std::string& cwd = fs.cwd());
}

TEST_F(SIGRunFilesystemTest, cwd_contents)
{
    FileSystem fs;
    ASSERT_NO_FATAL_FAILURE(fs.cwd_contents());
}

TEST_F(SIGRunFilesystemTest, cwd_contents_paths)
{
    FileSystem fs;
    const std::string path = "";

    ASSERT_NO_FATAL_FAILURE(fs.cwd_contents_paths(path));
}

TEST_F(SIGRunFilesystemTest, entry_type)
{
    FileSystem fs;
    const std::string path = "";

    ASSERT_NO_FATAL_FAILURE(fs.entry_type(path));

}