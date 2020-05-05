
#include "Scripting.hpp"
#include <string>
#include <fstream>
#include <streambuf>
#include <algorithm>
#include <numeric>
#include <QString>
#include <QStringList>
#include <boost/python.hpp>
#include <sigrun/log/Log.hpp>
#include "PythonInvoker.hpp"

namespace bp = boost::python;

Scripting::Scripting()
{
    HANDLE_PYTHON_CALL(
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
        d_globals["__builtins__"] = bp::import("builtins");
    )
}

Scripting::~Scripting()
{}

bp::object Scripting::si_plugin(std::string &module_name, std::string &path, std::string &class_name)
{
    HANDLE_PYTHON_CALL
    (
        return import(module_name, path).attr(class_name.c_str())();
    )

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

    remove_comments(ret);

    return ret;
}

void Scripting::load_class_names(std::vector<std::string> &classes, const std::string &path)
{
    uint64_t size = 0;

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

                if(class_name.substr(0, 2) != double_underscore && class_name != "SIEffect")
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
    HANDLE_PYTHON_CALL(
        bp::dict locals;

        locals["module_name"] = module;
        locals["path"] = path;

        bp::exec("import imp\n"
                 "new_module = imp.load_module(module_name, open(path), path, ('py', 'U', imp.PY_SOURCE))\n",
                 d_globals,
                 locals);

        return locals["new_module"];
    )

    return bp::object();
}

std::ostream& operator<<(std::ostream &os, const Scripting &scripting)
{
    return os << "Scripting Object: Globals: " << bp::extract<std::string>(bp::str(scripting.d_globals))() << " Main: " << bp::extract<std::string>(bp::str(scripting.d_main))();
}

std::string Scripting::remove_line_comments(const std::string& source)
{
    std::string temp = source;
    size_t cursor = std::string::npos;

    do
    {
        size_t hashtag = temp.find('#');
        size_t new_line = temp.find('\n', hashtag);

        if(hashtag != std::string::npos && new_line != std::string::npos)
        {
            temp.replace(hashtag, new_line - hashtag, "");

            cursor = hashtag - 1;
        }
        else
            cursor = std::string::npos;
    }
    while(cursor != std::string::npos);

    return temp;
}

std::string Scripting::remove_block_comments(const std::string& source)
{
    std::string temp = source;
    size_t cursor = std::string::npos;

    do
    {
        size_t left_triple_quotes = temp.find(R"(""")");
        size_t right_triple_quotes = temp.find(R"(""")", left_triple_quotes + 3);

        if(left_triple_quotes != std::string::npos && right_triple_quotes != std::string::npos)
        {
            temp.replace(left_triple_quotes, right_triple_quotes + 3 - left_triple_quotes, "");

            cursor = left_triple_quotes - 1;
        }
        else
            cursor = std::string::npos;
    }
    while(cursor != std::string::npos);

    return temp;
}

void Scripting::remove_comments(std::string& source)
{
    source = remove_line_comments(remove_block_comments(source));
}
