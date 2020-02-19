

#ifndef SITEST_FILESYSTEM_HPP
#define SITEST_FILESYSTEM_HPP

#include<boost/filesystem.hpp>
#include <sigrun/SIObject.hpp>

#ifdef __linux__
#include <pwd.h>
#endif

#ifdef _WIN32
#include <windows.h>
#include <Lmcons.h>
#endif // _WIN32

namespace fs = boost::filesystem;

class UserEnvironment
{
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

class FileSystem: public SIObject
{
public:
    FileSystem();
    ~FileSystem();



private:
bool find_directory(const fs::path& start_path, const fs::path& dir_name, fs::path& dir_path);
    fs::path d_cwd;
};


#endif //SITEST_FILESYSTEM_HPP
