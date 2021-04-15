
#ifndef SIGRUN_SCRIPTING_HPP
#define SIGRUN_SCRIPTING_HPP

#include <string>
#include <boost/python.hpp>
#include <ostream>
#include <sigrun/SIObject.hpp>

namespace bp = boost::python;

extern "C" PyObject* PyInit_libPySI(void);

class Scripting: public SIObject
{ SIGRUN
public:
    Scripting();
    ~Scripting();

    bp::object load_plugin(const std::string &module_name, const std::string &path);

    bp::object si_plugin(std::string &module_name, std::string &path, std::string &class_name);

    std::string load_plugin_source(const char *source);

    void load_class_names(std::vector<std::string> &classes, const std::string &path);

    friend std::ostream &operator<<(std::ostream &os, const Scripting &scripting);

private:

    void remove_comments(std::string& source);
    std::string remove_line_comments(const std::string& source);
    std::string remove_block_comments(const std::string& source);

    bp::object d_main;
    bp::object d_globals;
    std::string d_cwd;
};


#endif //SIGRUN_SCRIPTING_HPP
