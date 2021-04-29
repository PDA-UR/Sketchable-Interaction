
#include "Scripting.hpp"
#include <string>
#include <fstream>
#include <streambuf>
#include <algorithm>
#include <QString>
#include <QStringList>
#include <boost/python.hpp>
#include <sigrun/log/Log.hpp>
#include <glm/vec3.hpp>
#include "PythonInvoker.hpp"
#include <filesystem>

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
        d_cwd = directory;

        bp::exec((std::string("import builtins\nimport os\n\n") +
                              "os.remove(\"TEST.TXT\")\n" +
                              "open(\"TEST.TXT\", 'w').close()\n" +
                              "def si_print(filename):\n" +
                              "    def wrap(func):\n" +
                              "        def wrapped_func(*args, **kwargs):\n" +
                              "            with open(filename, \'a\') as outputfile:\n" +
                              "                outputfile.write(str(*args) + \",\" + str(**kwargs))\n" +
                              "            return func(\"PySI:\", *args, **kwargs)\n" +
                              "        return wrapped_func\n" +
                              "    return wrap\n\n" +
                              "builtins.print = si_print(\"TEST.TXT\")(builtins.print)\n"

                ).c_str(), d_globals);
    )
}

Scripting::~Scripting()
{

}

std::string Scripting::transpile(std::string& path, const std::string& path_addition)
{
    std::string source = load_plugin_source(path.c_str());

    std::vector<std::string> calls;

    std::vector<std::string> lines = str_split(source, '\n');
    std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_map<std::string, std::string>>> collision_events;

    int line_constructor_start = -1;
    int line_constructor_end = -1;

    for(int i = 0; i < lines.size(); ++i)
    {
        adjust_imports(lines[i], path_addition);

        if(line_constructor_end == -1)
            extract_line_constructor(line_constructor_start, line_constructor_end, lines, i);

        extract_calls(calls, collision_events, lines, i);
    }

    extract_calls(calls, collision_events);

    std::string src = "";

    lines.erase(remove_if(lines.begin(), lines.end(), [](const std::string& s)
    {
        return s.find("@SIEffect") != std::string::npos;
    }), lines.end());

    if(line_constructor_start != -1 && line_constructor_end != -1)
    {
        auto it = lines.begin() + line_constructor_end;

        for(int i = line_constructor_start; i < line_constructor_end; ++i)
            lines[i] = "\t" + lines[i];

        for(auto& call: calls)
            lines.insert(it, "\t" + call);

        it += calls.size();
        auto it_start = lines.begin() + line_constructor_start;

        lines.insert(it, std::string("        except Exception as ex:\n            print('Exception caught in file %s !' % __file__)\n            print(type(ex).__name__)\n            print(ex.args)"));
        lines.insert(it_start, std::string("        try:"));
    }

    for(auto& l: lines)
    {
        if(l.empty())
            continue;

        std::string s = "\t";
        std::string t = "    ";
        std::string::size_type n = 0;

        while((n = l.find(s, n)) != std::string::npos)
        {
            l.replace(n, s.size(), t);
            n += t.size();
        }

        src += (l + "\n");
    }

    return src;
}

void Scripting::extract_line_constructor(int& start, int& end,const std::vector<std::string>& lines, int i)
{
    if(lines[i].find("def __init__") != std::string::npos)
    {
        start = i + 1;

        for(int k = start; k < lines.size(); ++k)
        {
            if(lines[k].find("def ") != std::string::npos)
            {
                end = k - 1;
                return;
            }
        }

        end = lines.size() - 1;
    }
}

void Scripting::extract_calls(std::vector<std::string>& calls, std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_map<std::string, std::string>>>& collision_events, std::vector<std::string>& lines, int i)
{
    if (lines[i].find("@SIEffect") == std::string::npos)
        return;

    std::string event_type;
    std::string target_capability;
    std::string source_capability;
    std::string transmission_type;
    std::string target_function;

    std::string line = lines[i];
    line.erase(remove_if(line.begin(), line.end(), isspace), line.end());

    if (line.find("on_enter") != std::string::npos
        || line.find("on_continuous") != std::string::npos
        || line.find("on_leave") != std::string::npos)
    {
        std::vector tmp = str_split(line, ',');
        event_type = tmp[0].substr(tmp[0].find(".") + 1);
        event_type = event_type.substr(0, event_type.find("("));

        target_capability = tmp[0].substr(tmp[0].find("(") + 1);
        transmission_type = tmp[1].substr(0, tmp[1].find(")"));

        std::string temp = lines[i + 1];
        temp = temp.substr(temp.find_first_not_of(" \n\r\t\f\v"));

        target_function = str_split(temp, '(')[0];
        target_function = "self." + target_function.substr(target_function.find(" ") + 1);

        collision_events[target_capability][transmission_type][event_type] = target_function;
    }

    if (lines[i].find("on_link") != std::string::npos)
    {
        std::vector tmp = str_split(line, ',');

        std::string temp = lines[i + 1];
        temp = temp.substr(temp.find_first_not_of(" \n\r\t\f\v"));

        target_function = str_split(temp, '(')[0];
        target_function = "self." + target_function.substr(target_function.find(" ") + 1);

        std::string call;

        if (tmp.size() == 2)
        {
            target_capability = tmp[1].substr(0, tmp[1].find(")"));

            call = "\t\tself.enable_link_emission(" + target_capability + ", " + target_function + ")";
        }
        else
        {
            source_capability = tmp[1];
            target_capability = tmp[2].substr(0, tmp[2].find(")"));

            call = "\t\tself.enable_link_reception(" + source_capability + ", " + target_capability + ", " + target_function + ")";

        }

        calls.push_back(call);
    }
}

void Scripting::extract_calls(std::vector<std::string>& calls, const std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_map<std::string, std::string>>>& collision_events)
{
    for(auto& [capability, target]: collision_events)
    {
        for(auto& [transmission_event, function_data]: target)
        {
            std::string call = "\t\tself.enable_effect(" + capability + ", ";

            std::string on_enter = "None";
            std::string on_continuous = "None";
            std::string on_leave = "None";
            for(auto& [event_type, function]: function_data)
            {
                if(event_type == "on_enter")
                {
                    on_enter = function;
                }
                else if(event_type == "on_continuous")
                {
                    on_continuous = function;
                }
                else
                {
                    on_leave = function;
                }
            }

            call += (transmission_event + ", " + on_enter + ", " +  on_continuous + ", " + on_leave + ")");
            calls.push_back(call);
        }
    }
}

void Scripting::adjust_imports(std::string& s, const std::string& path_addition)
{
    if(s.find("from plugins.") != std::string::npos)
    {
        int p = s.find_first_of(".");
        s = s.substr(0, p) + "." + path_addition + s.substr(p);
    }
}


bp::object Scripting::si_plugin(std::string &module_name, std::string &path)
{
    std::string temp = path.substr(0, path.length() - 3); // -3 removes .py
    std::replace(temp.begin(), temp.end(), SI_SLASH_CHAR, SI_DOT_CHAR);
    bp::object ret = bp::import(bp::str((temp).c_str()));
    ret.attr(SI_INTERNAL_NAME) = module_name.c_str();

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

        cursor = std::string::npos;

        if(hashtag != std::string::npos && new_line != std::string::npos)
        {
            temp.replace(hashtag, new_line - hashtag, "");

            cursor = hashtag - 1;
        }
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

        cursor = std::string::npos;

        if(left_triple_quotes != std::string::npos && right_triple_quotes != std::string::npos)
        {
            temp.replace(left_triple_quotes, right_triple_quotes + 3 - left_triple_quotes, "");

            cursor = left_triple_quotes - 1;
        }
    }
    while(cursor != std::string::npos);

    return temp;
}

void Scripting::remove_comments(std::string& source)
{
    source = remove_line_comments(remove_block_comments(source));
}
