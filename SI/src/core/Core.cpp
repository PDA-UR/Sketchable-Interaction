

#include <debug/Print.hpp>
#include <python3_plugin/PluginCollector.hpp>
#include "python3_plugin/Plugin.hpp"
#include "Core.hpp"

#include <boost/python.hpp>

namespace bp = boost::python;

#ifdef xlib
Core::Core()
{
    Display* d = XOpenDisplay(nullptr);
    Screen*  s = DefaultScreenOfDisplay(d);

    d_target_width = s->width;
    d_target_height = s->height;

    d = nullptr;
    s = nullptr;
}
#else
Core::Core() : d_target_width(DEFAULT_WIDTH), d_target_height(DEFAULT_HEIGHT)
{

}
#endif

Core::~Core()
{
    delete p_ctx;
    p_ctx = nullptr;
}

void Core::start(const std::string& plugin_path)
{
    std::vector<std::string> files, classes;
    std::vector<bp::object> plugins;
    Scripting script;

    PluginCollector::collect("/" + plugin_path, files);

    for(auto& path : files)
    {
        std::string base_filename = path.substr(path.find_last_of("/\\") + 1);
        std::string module_name = base_filename.substr(0, base_filename.find_last_of('.'));
        std::string rpath = plugin_path + "/" + base_filename;

        script.load_class_names(classes, path);

        for(auto& ref : classes)
            plugins.push_back(script.si_plugin(module_name, rpath, ref));
    }

    p_ctx = new Context(d_target_width, d_target_height, plugins);
    p_ctx->begin();
}

void Core::stop()
{
    p_ctx->end();
}

const int Core::width() const
{
    return d_target_width;
}

const int Core::height() const
{
    return d_target_height;
}

const std::string &Core::plugin_path() const
{
    return d_plugin_path;
}
