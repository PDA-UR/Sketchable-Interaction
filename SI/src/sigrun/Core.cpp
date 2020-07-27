
#include <sigrun/log/Log.hpp>
#include <sigrun/rendering/IRenderEngine.hpp>

#include "Core.hpp"
#include "sigrun/plugin/Scripting.hpp"
#include "sigrun/plugin/PluginCollector.hpp"
#include <algorithm>
#include <execution>

/**
\brief constructor of Core class
\details
    Constructor which registers instance as an SIObject.
    Specify, which Logging capabilities are desired.
*/
Core::Core() = default;

/**
\brief destructor of Core class
\details
    Shut down the SIGRun environment.
*/
Core::~Core()
{
    INFO("Shutting down...");

    INFO("Shut down");
}

/**
\brief entry point of core SIGRun initialization
\details
    Entry point of SIGRun's core which performs Plugin loading and initializes the SI Context.
*/
void Core::start(char** argv, int argc, IRenderEngine* ire)
{
    SI_BENCHMARK_START;
    INFO("Initializing... ");

    std::unordered_map<std::string, std::unique_ptr<bp::object>> plugins;
    std::string path = "plugins";

    INFO("Loading plugins... ");
    retrieve_available_plugins(plugins, path);
    INFO("Loading plugins finished");

    if(!plugins.empty())
        INFO(std::to_string(plugins.size()) + " plugin(s) loaded");
    else
        ERROR("No plugins loaded");

    std::unique_ptr<Context> upctx(new Context());

    INFO("Initialization finished");

    upctx->begin(plugins, ire, argc, argv);
    INFO("Context closed");
}

/**
\brief exit SIGRun core
\details
    Initiate the shutdown of the SIGRun core.
*/
void Core::stop()
{

}

/**
\brief retrieve all available plugins before launching SIGRun environment
\details
    Load all plugins in the plugin path of the SIGRun environment.

@param plugins a mutable reference to a std::unordered map with std::string as key and a std::shared_ptr of boost::python::objects as values which is the out parameter
@param plugin_path a std::string which contains the path to the root folder of all plugin files

\see Scripting::Scripting
\see PluginCollector::PluginCollector
*/
void Core::retrieve_available_plugins(std::unordered_map<std::string, std::unique_ptr<bp::object>> &plugins, const std::string& plugin_path)
{
    std::vector<std::tuple<std::string, std::string>> files;

    Scripting script;

    PluginCollector().collect(SI_SLASH + plugin_path, files);

    for(const auto& file: files)
    {
        HANDLE_PYTHON_CALL(PY_ERROR, "Unknown Error.",
            std::vector<std::string> classes;

            const std::string& full_path = std::get<0>(file);
            const std::string& name = std::get<1>(file);

            std::string module_name = name.substr(0, name.find_last_of('.'));
            std::string rpath = full_path.substr(full_path.find(plugin_path)) + SI_SLASH + name;

            if(module_name != SI_PYTHON_STARTUP_FILE_NAME && module_name.substr(0, 2) != SI_DOUBLE_UNDERSCORE)
            {
                script.load_class_names(classes, rpath);

                for(auto& clazz: classes)
                {
                    bp::object obj = script.si_plugin(module_name, rpath, clazz);

                    HANDLE_PYTHON_CALL(PY_WARNING, "The found plugin does not contain the attribute \'regionname\' as a static class member (str-value). Therefore, the plugin is skipped and not available for use.",
                        plugins[std::string(bp::extract<char*>(obj.attr(obj.attr(SI_INTERNAL_NAME)).attr(SI_INTERNAL_REGION_NAME)))] = std::make_unique<bp::object>(obj);
                    )
                }
            }
        )
    }
}
