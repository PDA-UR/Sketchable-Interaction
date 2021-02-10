
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
#include <glm/vec3.hpp>
#include "PythonInvoker.hpp"

namespace bp = boost::python;

Scripting::Scripting()
{
    HANDLE_PYTHON_CALL(PY_ERROR, "Fatal Error.",
        PyImport_AppendInittab((char *) "libPySI", &PyInit_libPySI);

        Py_Initialize();

        INFO("Started Python3 Interpreter with Version: " + std::string(PY_VERSION));

        char buf[FILENAME_MAX];
        getcwd(buf, FILENAME_MAX);
        std::string directory(buf);

        bp::object sys_module = bp::import("sys");
        bp::object os_module = bp::import("os");


        bp::str module_directory = directory.c_str();
        sys_module.attr("path").attr("insert")(0, module_directory);

        d_main = bp::import("__main__");
        d_globals = d_main.attr("__dict__");
        d_globals["__builtins__"] = bp::import("builtins");


//        bp::exec("import sys\nprint(\"sys path: \", sys.path)", d_globals);


//        PyDict_SetItemString(d_globals.ptr(), "__builtins__", PyEval_GetBuiltins());

        bp::exec((std::string("class Unbuffered:\n") +
                 "   def __init__(self, stream):\n" +
                 "       self.stream = stream\n" +
                 "   def write(self, data):\n" +
                 "       self.stream.write(data)\n" +
                 "       self.stream.flush()\n" +
                 "   def writelines(self, datas):\n" +
                 "       self.stream.writelines(datas)\n" +
                 "       self.stream.flush()\n" +
                 "   def __getattr__(self, attr):\n" +
                 "       return getattr(self.stream, attr)\n" +
                 "\n" +
                 "import sys\n" +
                 "sys.stdout = open(\"TEST.TXT\", \"w\")\n" +
                 "sys.stdout = Unbuffered(sys.stdout)").c_str(), d_globals);

    )
}

Scripting::~Scripting()
{

}

bp::object Scripting::si_plugin(std::string &module_name, std::string &path, std::string &class_name)
{
    std::string temp = path.substr(0, path.length() - 3); // -3 removes .py
    std::replace(temp.begin(), temp.end(), SI_SLASH_CHAR, SI_DOT_CHAR);

    bp::object ret = bp::import(bp::str((temp).c_str()));
    ret.attr(SI_INTERNAL_NAME) = class_name.c_str();

    return ret;
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

    std::string clazz = SI_CLASS;
    std::string space = SI_WHITE_SPACE;

    while (size < source.size())
    {
        size_t found = source.find(clazz, size);

        if (found != std::string::npos)
        {
            size_t found2 = source.find(SI_BRACE_OPEN, found + 1);

            if (found2 != std::string::npos)
            {
                std::string class_name = source.substr(found + clazz.size() + space.size(),found2 - (found + clazz.size() + space.size()));

                if(class_name.substr(0, 2) != SI_DOUBLE_UNDERSCORE && class_name != SI_PYTHON_SIEFFECT_NAME && class_name != "E")
                    classes.push_back(class_name);

                size += found2;
            }
        }
        else
            break;
    }
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
        size_t hashtag = temp.find(SI_HASHTAG_CHAR);
        size_t new_line = temp.find(SI_NEW_LINE_CHAR, hashtag);

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
