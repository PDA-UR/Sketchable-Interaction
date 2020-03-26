
#include "Scripting.hpp"
#include <string>
#include <fstream>
#include <streambuf>
#include <boost/python.hpp>
#include <debug/Print.hpp>

namespace bp = boost::python;

Scripting::Scripting()
{
    PyImport_AppendInittab((char *) "libPySI", &PyInit_libPySI);

    Py_Initialize();

    char buf[FILENAME_MAX];
    getcwd(buf, FILENAME_MAX);
    std::string directory(buf);

    bp::object sys_module = bp::import("sys");
    bp::str module_directory = directory.substr(0, directory.size() - 2).c_str();
    sys_module.attr("path").attr("insert")(1, module_directory);

    d_main = bp::import("__main__");
    d_globals = d_main.attr("__dict__");
}

Scripting::~Scripting()
{}

bp::object Scripting::si_plugin(std::string &module_name, std::string &path, std::string &class_name)
{
    try
    {
        return import(module_name, path).attr(class_name.c_str())();
    }
    catch(bp::error_already_set&)
    {}

    return bp::object();
}

std::string Scripting::load_plugin_source(const char *source)
{
    std::ifstream file(source);
    std::string ret;

    file.seekg(0, std::ios::end);
    ret.reserve(file.tellg());
    file.seekg(0, std::ios::beg);

    ret.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());

    return ret;
}

void Scripting::load_class_names(std::vector<std::string> &classes, const std::string &path)
{
    int size = 0;

    std::string source = load_plugin_source(path.c_str());

    std::string clazz = "class";
    std::string space = " ";
    std::string brace = "(";
    std::string double_underscore = "__";

    while (size < source.size())
    {
        size_t found = source.find(clazz, size);

        if (found != std::string::npos)
        {
            size_t found2 = source.find(brace, found + 1);

            if (found2 != std::string::npos)
            {
                std::string class_name = source.substr(found + clazz.size() + space.size(),found2 - (found + clazz.size() + space.size()));

                if(class_name.substr(0, 2) != double_underscore)
                    classes.push_back(class_name);

                size += found2;
            }
        }
        else
            break;
    }
}

bp::object Scripting::import(const std::string &module, const std::string &path)
{
    bp::dict locals;

    locals["module_name"] = module;
    locals["path"] = path;

    bp::exec("import imp\n"
             "new_module = imp.load_module(module_name, open(path), path, ('py', 'U', imp.PY_SOURCE))\n",
             d_globals,
             locals);

    return locals["new_module"];
}

std::ostream &operator<<(std::ostream &os, const Scripting &scripting)
{
    return os << "Scripting Object: Globals: " << bp::extract<std::string>(bp::str(scripting.d_globals))()
              << " Main: " << bp::extract<std::string>(bp::str(scripting.d_main))();
}
