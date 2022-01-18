
#include <sigrun/log/Log.hpp>
#include <sigrun/rendering/IRenderEngine.hpp>

#include "Core.hpp"
#include "sigrun/plugin/PluginCollector.hpp"
#include <algorithm>
#include <execution>
#include <filesystem>
#include <e/E.hpp>

namespace sfs = std::filesystem;


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

    Py_Finalize();

    INFO("Shut down");
}

/**
\brief entry point of core SIGRun initialization
\details
    Entry point of SIGRun's core which performs Plugin loading and initializes the SI Context.
*/
void Core::start(char** argv, int argc, IRenderEngine* ire, IROS2Environment* ros)
{
    SI_BENCHMARK_START;

    std::unique_ptr<Context> upctx(new Context());

    INFO("Initializing... ");

    INFO("Generating E class... ");
    E::generate();
    INFO("Generated E class");

    std::unordered_map<std::string, std::unique_ptr<bp::object>> plugins;
    std::string path = "plugins";
    std::ofstream ofs(sfs::path(".TEST.TXT"));
    ofs.close();

    INFO("Loading plugins... ");
    retrieve_available_plugins(plugins, path);
    INFO("Loading plugins finished");

    if(!plugins.empty())
        INFO(std::to_string(plugins.size()) + " plugin(s) loaded");
    else
        ERROR("No plugins loaded");


    INFO("Initialization finished");

    upctx->begin(plugins, ire, ros, argc, argv);
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
    std::vector<std::tuple<std::string, std::string>> to_load;

    Scripting script;
    std::string path_addition = "__loaded_plugins__";

    sfs::remove_all(sfs::path(plugin_path + "/" + path_addition));
    sfs::create_directory(sfs::path(plugin_path + "/" + path_addition));

    PluginCollector().collect(SI_SLASH + plugin_path, files);

    prepare_plugin_loading(to_load, files, plugin_path, path_addition, script);
    load_plugins(plugins, to_load, script);
}

void Core::prepare_plugin_loading(std::vector<std::tuple<std::string, std::string>>& to_load, const std::vector<std::tuple<std::string, std::string>>& files, const std::string& plugin_path, const std::string& path_addition, Scripting& script)
{
    for(const auto& file: files)
        process_plugin_file(to_load, path_addition, file, script);
}

void Core::load_plugins(std::unordered_map<std::string, std::unique_ptr<bp::object>>& plugins, const std::vector<std::tuple<std::string, std::string>>& to_load, Scripting& script)
{
    for(auto& t: to_load)
    {
        std::string module_name = std::get<0>(t);
        std::string path = std::get<1>(t);

        if (module_name != SI_PYTHON_STARTUP_FILE_NAME && module_name.substr(0, 2) != SI_DOUBLE_UNDERSCORE
            && module_name != "E" && module_name != SI_PYTHON_SIEFFECT_NAME)
        {
            HANDLE_PYTHON_CALL(PY_ERROR, "Could not load plugin!",
               bp::object o = script.si_plugin(module_name, path);
               plugins[std::string(bp::extract<char *>(o.attr(o.attr(SI_INTERNAL_NAME)).attr(SI_INTERNAL_REGION_NAME)))] = std::make_unique<bp::object>(o);
            )
        }
    }
}

void Core::process_plugin_file(std::vector<std::tuple<std::string, std::string>>& to_load, const std::string& path_addition, const std::tuple<std::string, std::string>& file, Scripting& script)
{
    const std::string &name = std::get<1>(file);
    std::string p = std::get<0>(file) + "/" + name;
    std::string source = script.transpile(p, path_addition);
    std::string path = std::get<0>(file).substr(std::get<0>(file).find("plugins"));
    std::string loaded_path = (path == "plugins") ? "plugins/" + path_addition : path.substr(0, path.find_first_of("/")) + "/" + path_addition + path.substr(path.find_first_of("/"));

    filesystem_operations(loaded_path, path, name, source);

    std::string module_name = name.substr(0, name.find_last_of('.'));
    std::string rpath = loaded_path + SI_SLASH + name;

    to_load.emplace_back(module_name, rpath);
}

void Core::filesystem_operations(const std::string& loaded_path, const std::string& path, const std::string& name, const std::string& source)
{
    sfs::create_directories(sfs::path(loaded_path));

    create_transpiled_plugin_files(source, loaded_path, name);
    copy_qml_and_res(path, loaded_path);
}

void Core::create_transpiled_plugin_files(const std::string& source, const std::string& loaded_path, const std::string& name)
{
    std::ofstream ofs(sfs::path(loaded_path + "/" + name));
    ofs << source;
    ofs.close();
}

void Core::copy_qml_and_res(const std::string& path, const std::string& loaded_path)
{
    for(auto const & e: sfs::directory_iterator(sfs::path(path)))
    {
        if(sfs::is_regular_file(e) && e.path().extension() == ".qml")
        {
            if(!sfs::exists(sfs::path(loaded_path + "/" + e.path().filename().string())))
                sfs::copy(e.path(), sfs::path(loaded_path + "/" + e.path().filename().string()));
        }

        if(sfs::is_directory(e) && e.path().filename().string() == "res")
        {
            if(!sfs::exists(sfs::path(loaded_path + "/res")))
                sfs::copy(e.path(), sfs::path(loaded_path + "/res"));
        }
    }
}