
#include <sigrun/log/Log.hpp>

#include "Core.hpp"
#include "sigrun/plugin/Scripting.hpp"
#include "sigrun/plugin/PluginCollector.hpp"

Core::Core()
{META_TYPE("CORE")
    Log::__DEBUG__ = true;
    Log::SHOW = LOG_SHOW_ALL;
}

Core::~Core()
{
    INFO("Shutting down... ", LOG_CONSOLE);
    INFO("Shut down", LOG_CONSOLE);
}

void Core::start()
{
    INFO("Initializing... ", LOG_CONSOLE);

    std::unordered_map<std::string, std::shared_ptr<bp::object>> plugins;
    std::string path = "plugins/standard_environment_library/";

    INFO("Loading plugins... ", LOG_CONSOLE);
    retrieve_available_plugins(plugins, path);
    INFO("Loading plugins finished", LOG_CONSOLE);

    if(!plugins.empty())
        INFO(std::to_string(plugins.size()) + " plugin(s) loaded", LOG_CONSOLE);
    else
        ERROR("No plugins loaded", LOG_CONSOLE);

    INFO("Initialization finished", LOG_CONSOLE);
}

void Core::stop()
{

}

void Core::retrieve_available_plugins(std::unordered_map<std::string, std::shared_ptr<bp::object>> &plugins, const std::string& plugin_path)
{
    std::vector<std::string> files;
    Scripting script;

    PluginCollector().collect("/" + plugin_path, files);

    for (auto &path : files)
    {
        std::vector<std::string> classes;
        std::string base_filename = path.substr(path.find_last_of("/\\") + 1);
        std::string module_name = base_filename.substr(0, base_filename.find_last_of('.'));
        std::string rpath = plugin_path + base_filename;

        script.load_class_names(classes, path);

        for (auto &ref : classes)
            plugins[ref] = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, ref));
    }
}
