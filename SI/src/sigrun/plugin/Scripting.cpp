
#include "Scripting.hpp"
#include <iostream>
#include <string>
#include <iosfwd>
#include <boost/python.hpp>
#include <debug/Print.hpp>

namespace bp = boost::python;

Scripting::Scripting()
{
    PyImport_AppendInittab((char *) "libPySI", &PyInit_libPySI);

    Py_Initialize();

    d_main = bp::import("__main__");
    d_globals = d_main.attr("__dict__");
}

Scripting::~Scripting()
{}

bp::object Scripting::si_plugin(std::string &module_name, std::string &path, std::string &class_name)
{
    return import(module_name, path).attr(class_name.c_str())();
}

std::string Scripting::load_plugin_source(const char *source)
{
    char *buffer = nullptr;
    long length;

    FILE *f = fopen(source, "rb");

    if (f)
    {
        fseek(f, 0, SEEK_END);

        length = ftell(f);

        fseek(f, 0, SEEK_SET);

        buffer = (char *) malloc(length);

        if (buffer)
        {
            buffer[0] = '\0';

            fread(buffer, 1, length, f);

            buffer[length] = '\0';
        }

        fclose(f);
    }

    std::string ret(strdup(buffer));

    free(buffer);

    return ret;
}

void Scripting::load_class_names(std::vector<std::string> &classes, const std::string &path)
{
    int size = 0;

    std::string source = load_plugin_source(path.c_str());

    std::string clazz = "class";
    std::string space = " ";
    std::string brace = "(";

    while (size < source.size())
    {
        size_t found = source.find(std::string("class"), size);

        if (found != std::string::npos)
        {
            size_t found2 = source.find(std::string("("), found + 1);

            if (found2 != std::string::npos)
            {
                classes.push_back(source.substr(found + clazz.size() + space.size(),found2 - (found + clazz.size() + space.size())));

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
