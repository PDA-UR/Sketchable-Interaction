
#ifndef SIGRUN_PLUGINCOLLECTOR_HPP
#define SIGRUN_PLUGINCOLLECTOR_HPP

#include <string>
#include <vector>
#include <sigrun/MetaType.hpp>

class PluginCollector: public SIMetaType
{
public:
    PluginCollector()
    {META_TYPE("PLUGIN_COLLECTOR")}

    ~PluginCollector() = default;
    void collect(const std::string &rel_path, std::vector<std::string> &files);

private:
    std::string plugin_folder(const std::string &rel_path);
    void grab_plugin_files(const std::string &path, std::vector<std::string> &files);
};

#endif //SIGRUN_PLUGINCOLLECTOR_HPP
