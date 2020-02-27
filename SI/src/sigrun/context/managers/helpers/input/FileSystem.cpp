
#include "FileSystem.hpp"
#include <sigrun/log/Log.hpp>

FileSystemObject::FileSystemObject(const fs::path &path, int type):
    d_path(path),
    d_type(type)
{SIGRUN
}

FileSystemObject::~FileSystemObject() = default;

const std::string &FileSystemObject::path()
{
    return d_path.string();
}

const std::string &FileSystemObject::filename()
{
    return d_path.filename().string();
}

const std::string &FileSystemObject::extension()
{
    return d_path.extension().string();
}

FileSystem::FileSystem()
{ SIGRUN
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
    std::vector<std::string> ret;
    ret.reserve(d_cwd_contents.size());

    for(auto& path: d_cwd_contents)
        ret.push_back(path->path());

    return ret;
}

int FileSystem::entry_type(const std::string& _path)
{
    fs::path path(_path);

    if (fs::exists(path) && fs::is_regular_file(path))
    {
        const std::string& ext = path.extension().string();

        if(ext == ".png" || ext == ".jpeg" || ext == ".jpg")
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
        for (const auto& entry : fs::directory_iterator(path))
        {
            if (fs::is_directory(entry.status()))
            {
                d_cwd_contents.emplace_back(std::make_shared<FileSystemObject>(entry.path(), SI_TYPE_DIRECTORY));
            }
            else if (fs::is_regular_file(entry.status()))
            {
                const std::string& ext = entry.path().extension().string();

                int type = SI_TYPE_UNKNOWN_FILE;

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
