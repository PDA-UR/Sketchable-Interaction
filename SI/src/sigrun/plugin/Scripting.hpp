
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

    std::string transpile(std::string& path, const std::string& path_addition);

    bp::object si_plugin(std::string &module_name, std::string &path);



    friend std::ostream &operator<<(std::ostream &os, const Scripting &scripting);

private:

    void extract_superclasses(std::vector<std::string>& superclasses, const std::string& line);
    void extract_line_constructor(int& start, int& end, const std::vector<std::string>& lines, int n);
    void extract_calls(std::vector<std::string>& calls, std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_map<std::string, std::string>>>& collision_events, std::vector<std::string>& lines, int n);
    void extract_calls(std::vector<std::string>& calls, const std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_map<std::string, std::string>>>& collision_events);
    std::string load_plugin_source(const char *source);

    std::vector<std::string> d_conditionals;

    void adjust_imports(std::string& s, const std::string& path_addition);

    std::vector<std::string> str_split(const std::string& str, char delim) {
        std::vector<std::string> strings;
        size_t start;
        size_t end = 0;

        while ((start = str.find_first_not_of(delim, end)) != std::string::npos) {
            end = str.find(delim, start);
            strings.push_back(str.substr(start, end - start));
        }

        return strings;
    }

    void remove_comments(std::string& source);
    std::string remove_line_comments(const std::string& source);
    std::string remove_block_comments(const std::string& source);

    bp::object d_main;
    bp::object d_globals;
    std::string d_cwd;
};


#endif //SIGRUN_SCRIPTING_HPP
