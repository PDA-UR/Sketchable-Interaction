
#include "FileSystem.hpp"
#include <sigrun/log/Log.hpp>
#include <execution>

FileSystemObject::FileSystemObject(const fs::path &path, uint32_t type):
    d_path(path),
    d_type(type)
{
}

FileSystemObject::~FileSystemObject() = default;

const std::string FileSystemObject::path()
{
    return d_path.string();
}

const std::string FileSystemObject::filename()
{
    return d_path.filename().string();
}

const std::string FileSystemObject::extension()
{
    return d_path.extension().string();
}

FileSystem::FileSystem()
{
    INFO("Acessing filesystem...");
#ifdef __linux__
    set_cwd("/home/" + UserEnvironment::user_name() + "/Desktop");
#endif // __linux__

#ifdef _WIN32
    set_cwd("C:\\Users\\" + UserEnvironment::user_name() + "\\Desktop\\");
#endif // _WIN32


    INFO("Current working directory set to " + d_cwd.string());
}

FileSystem::~FileSystem() = default;

void FileSystem::set_cwd(const std::string &path)
{
    set_cwd(fs::path(path));
}

const std::string &FileSystem::cwd() const
{
    return d_cwd.string();
}

const std::vector<std::shared_ptr<FileSystemObject>> &FileSystem::cwd_contents()
{
    return d_cwd_contents;
}

const std::vector<std::string> FileSystem::cwd_contents_paths(const std::string& cwd)
{
    set_cwd(cwd);
    std::vector<std::string> ret(d_cwd_contents.size());

    std::transform(std::execution::par_unseq, d_cwd_contents.begin(), d_cwd_contents.end(), ret.begin(), [](auto& content) -> std::string
    {
        return content->path();
    });

    return ret;
}

const std::vector<int> FileSystem::cwd_contents_types(const std::vector<std::string>& contents)
{
    std::vector<int> types;
    types.reserve(contents.size());

    std::transform(contents.begin(), contents.end(), std::back_inserter(types), [&](auto& path)
    {
        return entry_type(path);
    });

    return types;
}

uint32_t FileSystem::entry_type(const std::string& _path)
{
    fs::path path(_path);

    if (fs::exists(path) && fs::is_regular_file(path))
    {
        const std::string& ext = path.extension().string();

        if(ext == ".png" || ext == ".jpeg" || ext == ".jpg" || ext == ".gif")
            return SI_TYPE_IMAGE_FILE;
        else if(ext == ".txt" || ext == ".odt" || ext == ".md")
            return SI_TYPE_TEXT_FILE;
    }
    else if (fs::exists(path) && fs::is_directory(path))
        return SI_TYPE_DIRECTORY;

    return SI_TYPE_UNKNOWN_FILE;
}

void FileSystem::set_cwd(const fs::path &path)
{
    d_cwd_contents.clear();
    d_cwd = path;

    if (fs::exists(path) && fs::is_directory(path))
    {
        for(const auto& entry : fs::directory_iterator(path))
        {
            if (fs::is_directory(entry.status()))
            {
                d_cwd_contents.emplace_back(std::make_shared<FileSystemObject>(entry.path(), SI_TYPE_DIRECTORY));
            }
            else if (fs::is_regular_file(entry.status()))
            {
                const std::string& ext = entry.path().extension().string();

                uint32_t type = SI_TYPE_UNKNOWN_FILE;

                if(ext == ".png" || ext == ".jpeg" || ext == ".jpg")
                    type = SI_TYPE_IMAGE_FILE;
                else if(ext == ".txt" || ext == ".odt" || ext == ".md")
                    type = SI_TYPE_TEXT_FILE;

                d_cwd_contents.emplace_back(std::make_shared<FileSystemObject>(entry.path(), type));
            }
            else
            {
                d_cwd_contents.emplace_back(std::make_shared<FileSystemObject>(entry.path(), SI_TYPE_UNKNOWN_FILE));
            }
        }
    }
}

bool FileSystem::find_directory(const fs::path& start_path, const fs::path& dir_name, fs::path& dir_path)
{
    const fs::recursive_directory_iterator end;

    const auto it = std::find_if(fs::recursive_directory_iterator(start_path), end, [&dir_name](const fs::directory_entry& e)
    {
        return e.path().filename() == dir_name;
    });

    if (it == end)
        return false;

    dir_path = it->path();

    return true;
}
