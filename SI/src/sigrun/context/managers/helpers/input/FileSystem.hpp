

#ifndef SITEST_FILESYSTEM_HPP
#define SITEST_FILESYSTEM_HPP

#include <boost/filesystem.hpp>
#include <sigrun/SIObject.hpp>
#include <memory>
#include <sigrun/SITypes.hpp>

#ifdef __linux__
#include <pwd.h>
#endif

#ifdef _WIN32
#include <windows.h>
#include <Lmcons.h>
#endif // _WIN32

namespace fs = boost::filesystem;

class UserEnvironment: public SIObject
{ SIGRUN
public:

#ifdef __linux__
    static std::string user_name()
    {
        return std::string(getpwuid(geteuid())->pw_name);
    }

#endif //__linux__

#ifdef _WIN32
    static std::string user_name()
    {
        TCHAR name [ UNLEN + 1 ];
        DWORD size = UNLEN + 1;

        if (GetUserName((TCHAR*)name, &size))
            return std::string(name);
    }
#endif // _WIN32
};

class FileSystemObject: public SIObject
{ SIGRUN
public:
    FileSystemObject(const fs::path& path, uint32_t type);
    ~FileSystemObject();

    const std::string path();
    const std::string filename();
    const std::string extension();

private:
    uint32_t d_type;
    fs::path d_path;
};

class FileSystem: public SIObject
{ SIGRUN
public:
    FileSystem();
    ~FileSystem();

    void set_cwd(const std::string& path);
    const std::string& cwd() const;
    const std::vector<std::shared_ptr<FileSystemObject>>& cwd_contents();
    const std::vector<std::string> cwd_contents_paths(const std::string& cwd);
    const std::vector<int> cwd_contents_types(const std::vector<std::string>& contents);

    uint32_t entry_type(const std::string& path);

private:
    void set_cwd(const fs::path& path);

    bool find_directory(const fs::path& start_path, const fs::path& dir_name, fs::path& dir_path);

    fs::path d_cwd;

    std::vector<std::shared_ptr<FileSystemObject>> d_cwd_contents;
};


#endif //SITEST_FILESYSTEM_HPP
