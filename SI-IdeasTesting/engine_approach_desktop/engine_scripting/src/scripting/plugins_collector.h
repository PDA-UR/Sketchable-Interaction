
#ifndef ENGINE_SCRIPTING_PLUGINS_COLLECTOR_H
#define ENGINE_SCRIPTING_PLUGINS_COLLECTOR_H

#include <cstdio>
#include <unistd.h>
#include <dirent.h>
#include <string>
#include <functional>
#include <cstring>
#include <vector>

class plugins_collector
{
public:
    static void collect(std::vector<std::string>& files);
private:
    static std::string plugin_folder();
    static void grab_plugin_files(const std::string &path, std::vector<std::string>& files);
};


#endif //ENGINE_SCRIPTING_PLUGINS_COLLECTOR_H
