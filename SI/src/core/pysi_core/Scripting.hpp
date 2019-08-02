

#ifndef SI_SCRIPTING_HPP
#define SI_SCRIPTING_HPP

#include <boost/python.hpp>
#include <ostream>

namespace bp = boost::python;

extern "C" PyObject* PyInit_libPySI(void);

class Scripting
{
public:
    Scripting();
    ~Scripting();

    bp::object si_plugin(std::string& module_name, std::string& path, std::string& class_name);

    std::string load_plugin_source(const char* source);

    void load_class_names(std::vector<std::string>& classes, const std::string& path);
    bp::object import(const std::string& module, const std::string& path);

    friend std::ostream &operator<<(std::ostream &os, const Scripting &scripting);

private:

    bp::object d_main;
    bp::object d_globals;
};


#endif //SI_SCRIPTING_HPP
