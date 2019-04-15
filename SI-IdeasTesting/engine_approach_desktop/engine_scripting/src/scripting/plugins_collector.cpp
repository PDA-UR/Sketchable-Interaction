
#include "plugins_collector.h"

void plugins_collector::collect(std::vector<std::string> &files)
{
    plugins_collector::grab_plugin_files(plugins_collector::plugin_folder(), files);
}

std::string plugins_collector::plugin_folder()
{
    char buf[FILENAME_MAX];

    getcwd(buf, FILENAME_MAX);

    std::string directory(buf);
    directory += "/plugins";

    return directory;
}

void plugins_collector::grab_plugin_files(const std::string &path, std::vector<std::string> &files)
{
    if (auto dir = opendir(path.c_str()))
    {
        while(auto f = readdir(dir))
        {
            if (!f->d_name || f->d_name[0] == '.')
                continue;

            if (f->d_type == DT_DIR)
                grab_plugin_files(path + f->d_name + "/", files);

            if (f->d_type == DT_REG)
            {
                if(strstr(f->d_name, "__init__.py"))
                    continue;

                if(strstr(f->d_name, ".py"))
                    files.push_back(path + "/" + f->d_name);
            }
        }

        closedir(dir);
    }
}
