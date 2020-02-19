
#include "FileSystem.hpp"
#include <sigrun/log/Log.hpp>

FileSystem::FileSystem()
{ SIGRUN
    INFO("Acessing filesystem...");

#ifdef __linux__
    d_cwd = fs::path("/home/" + UserEnvironment::user_name() + "/Desktop");
#endif // __linux__

#ifdef _WIN32
    d_cwd = fs::path("C:\\Users\\" + UserEnvironment::user_name() + "\\Desktop\\");
#endif // _WIN32

    INFO("Current working directory set to " + d_cwd.string());
}

FileSystem::~FileSystem() = default;

bool FileSystem::find_directory(const fs::path& start_path, const fs::path& dir_name, fs::path& dir_path)
{
    const fs::recursive_directory_iterator end;
    const auto it = std::find_if(fs::recursive_directory_iterator(start_path), end,
    [&dir_name](const fs::directory_entry& e)
    {
        return e.path().filename() == dir_name;
    });

    if (it == end)
        return false;
    else
        dir_path = it->path();
        return true;
}
