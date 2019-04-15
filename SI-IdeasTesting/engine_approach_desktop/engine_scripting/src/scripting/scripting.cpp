
#include "scripting.h"

scripting::scripting()
{
    PyImport_AppendInittab((char*) "libengine_scripting_lib", &PyInit_libengine_scripting_lib);

    Py_Initialize();

    main = bp::import("__main__");
    globals = main.attr("__dict__");
}

bp::object scripting::region_plugin(std::string &module_name, std::string &path, std::string &class_name)
{
    bp::object module = import(module_name, path);
    bp::object PyRegion = module.attr(class_name.c_str());

    bp::object pyreg = PyRegion();

    return pyreg;
}

std::string scripting::load_plugin_source(const char *source)
{
    char * buffer = nullptr;
    long length;

    FILE * f = fopen(source, "rb");

    if(f)
    {
        fseek(f, 0, SEEK_END);

        length = ftell(f);

        fseek(f, 0, SEEK_SET);

        buffer = (char*) malloc(length);

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

void scripting::load_class_names(std::vector<std::string> &classes, std::string &path)
{
    int size = 0;

    std::string source = load_plugin_source(path.c_str());

    while(size < source.size())
    {
        size_t found = source.find(std::string("class"), size);

        if (found != std::string::npos)
        {
            size_t found2 = source.find(std::string("("), found + 1);

            if(found2 != std::string::npos)
            {
                classes.push_back(source.substr(found + 5 + 1, found2 - (found + 5 + 1)));

                size += found2;
            }
        }
        else
            break;
    }
}

bp::object scripting::import(const std::string &module, const std::string &path)
{
    bp::dict locals;

    locals["module_name"] = module;
    locals["path"] = path;

    bp::exec("import imp\n"
             "new_module = imp.load_module(module_name, open(path), path, ('py', 'U', imp.PY_SOURCE))\n",
             globals,
             locals);

    return locals["new_module"];}
