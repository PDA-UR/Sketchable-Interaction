
#ifndef ENGINE_SCRIPTING_SCRIPTING_H
#define ENGINE_SCRIPTING_SCRIPTING_H

#include <boost/python.hpp>

#include <iostream>

namespace bp = boost::python;

extern "C" PyObject* PyInit_libengine_scripting_lib(void); // macro this for dynamic naming?

class scripting
{
public:
    scripting();
    ~scripting() = default;

    bp::object region_plugin(std::string& module_name, std::string& path, std::string& class_name);

    std::string load_plugin_source(const char* source);

    void load_class_names(std::vector<std::string>& classes, std::string& path);

private:
    bp::object import(const std::string& module, const std::string& path);

    bp::object main;
    bp::object globals;
};


#endif //ENGINE_SCRIPTING_SCRIPTING_H
