
#include <zconf.h>
#include <dirent.h>
#include <cstring>
#include "PluginCollector.hpp"
#include "sigrun/log/Log.hpp"
#include <sigrun/SITypes.hpp>

void PluginCollector::collect(const std::string &rel_path, std::vector<std::tuple<std::string, std::string>> &files)
{
    PluginCollector::grab_plugin_files(PluginCollector::plugin_folder(rel_path), files);
}

std::string PluginCollector::plugin_folder(const std::string &rel_path)
{
    char buf[FILENAME_MAX];
    getcwd(buf, FILENAME_MAX);
    std::string directory(buf);
    directory += rel_path;

    return directory;
}

void PluginCollector::grab_plugin_files(const std::string &path, std::vector<std::tuple<std::string, std::string>> &files)
{
    if (auto dir = opendir(path.c_str()))
    {
        while (auto f = readdir(dir))
        {
            if (f->d_name[0] == SI_DOT_CHAR || !f->d_name)
                continue;

            if (f->d_type == DT_DIR)
                if(!strstr(f->d_name, SI_PYCACHE))
                    grab_plugin_files(path + SI_SLASH + f->d_name, files);

            if (f->d_type == DT_REG)
            {
                if (!strstr(f->d_name, SI_PYTHON_INIT_PY))
                    if (strstr(f->d_name, SI_PYTHON_PY_ENDING))
                        files.emplace_back(path, f->d_name);
            }
        }

        closedir(dir);
    }
}