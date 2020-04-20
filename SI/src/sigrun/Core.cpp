
#include <sigrun/log/Log.hpp>
#include <sigrun/rendering/IRenderEngine.hpp>

#include "Core.hpp"
#include "sigrun/plugin/Scripting.hpp"
#include "sigrun/plugin/PluginCollector.hpp"
#include <algorithm>
#include <execution>

/**
\brief constructor
\details
    Constructor which registers instance as an SIObject.
    Specify, which Logging capabilities are desired.
*/
Core::Core() = default;

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

    PluginCollector().collect("/" + plugin_path, files);

    std::transform(std::execution::seq, files.begin(), files.end(), files.begin(), [&](auto& file)
    {
        std::vector<std::string> classes;

        const std::string& full_path = std::get<0>(file);
        const std::string& name = std::get<1>(file);

        std::string module_name = name.substr(0, name.find_last_of('.'));
        std::string rpath = full_path.substr(full_path.find(plugin_path)) + "/" + name;

        script.load_class_names(classes, rpath);

        HANDLE_PYTHON_CALL(
            std::transform(std::execution::par_unseq, classes.begin(), classes.end(), classes.begin(), [&](auto& clazz)
            {
                bp::object obj = script.si_plugin(module_name, rpath, clazz);

                const char* name = bp::extract<char*>(obj.attr("name"));

                plugins[std::string(name)] = std::make_unique<bp::object>(obj);
                return clazz;
            });
        )

       return file;
    });
}
