

#ifndef SI_PLUGINCOLLECTOR_HPP
#define SI_PLUGINCOLLECTOR_HPP


#include <cstdio>
#include <unistd.h>
#include <zconf.h>
#include <dirent.h>
#include <vector>
#include <string>
#include <string>
#include <functional>
#include <cstring>

class PluginCollector
{
public:
    static void collect(const std::string& rel_path, std::vector<std::string>& files);
private:
    static std::string plugin_folder(const std::string& rel_path);
    static void grab_plugin_files(const std::string &path, std::vector<std::string>& files);
};


#endif //SI_PLUGINCOLLECTOR_HPP
