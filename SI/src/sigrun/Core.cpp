
#include <sigrun/log/Log.hpp>
#include <sigrun/rendering/IRenderEngine.hpp>

#include "Core.hpp"
#include "sigrun/plugin/Scripting.hpp"
#include "sigrun/plugin/PluginCollector.hpp"

/**
\brief constructor
\details
    Constructor which registers instance as an SIObject.
    Specify, which Logging capabilities are desired.
*/
Core::Core()
{SIOBJECT
    Log::__DEBUG__ = true;
    Log::SHOW = LOG_SHOW_ALL;
    Log::WHERE = LOG_CONSOLE;
}

/**
\brief destructor
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
    INFO("Initializing... ");

    std::unordered_map<std::string, std::unique_ptr<bp::object>> plugins;
    std::string path = std::string(argv[1]) + "/standard_environment_library/";

    INFO("Loading plugins... ");
    retrieve_available_plugins(plugins, path);
    INFO("Loading plugins finished");

    if(!plugins.empty())
        INFO(std::to_string(plugins.size()) + " plugin(s) loaded");
    else
        ERROR("No plugins loaded");

    std::unique_ptr<Context> upctx(new Context(1920, 1080, plugins));

    INFO("Initialization finished");

    upctx->begin(ire, argc, argv);
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
            plugins[ref] = std::make_unique<bp::object>(script.si_plugin(module_name, rpath, ref));
    }
}
