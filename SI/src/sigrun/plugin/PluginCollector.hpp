
#ifndef SIGRUN_PLUGINCOLLECTOR_HPP
#define SIGRUN_PLUGINCOLLECTOR_HPP

#include <string>
#include <vector>
#include <sigrun/SIObject.hpp>

class PluginCollector: public SIObject
{ SIGRUN
public:
    PluginCollector() = default;
    ~PluginCollector() = default;

    void collect(const std::string &rel_path, std::vector<std::tuple<std::string, std::string>> &files);

private:
    std::string plugin_folder(const std::string &rel_path);
    void grab_plugin_files(const std::string &path, std::vector<std::tuple<std::string, std::string>> &files);
};

#endif //SIGRUN_PLUGINCOLLECTOR_HPP
