

#include "SIGRunPythonInvokerTest.hpp"

#include <sigrun/plugin/PythonInvoker.hpp>
#include <sigrun/plugin/Scripting.hpp>
#include <sigrun/plugin/PluginCollector.hpp>
#include <boost/python.hpp>
#include <pysi/SuperEffect.hpp>

namespace bp = boost::python;

TEST_F(SIGRunPythonInvokerTest, construction)
{
    EXPECT_NO_FATAL_FAILURE(PythonInvoker pyi);
}

TEST_F(SIGRunPythonInvokerTest, invoke_extract_attribute_success_primitives)
{
    PythonInvoker pyi;
    Scripting script;

    const std::string rel_path = "res/python_invoker";
    std::vector<std::string> files, classes;

    PluginCollector().collect("/" + rel_path, files);

    std::string base_filename = files[0].substr(files[0].find_last_of("/\\") + 1);
    std::string module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    std::string rpath = rel_path + "/" + base_filename;

    script.load_class_names(classes, files[0]);

    bp::object x = script.si_plugin(module_name, rpath, classes[0]);

    int i11 = pyi.invoke_extract_attribute<int>(x, "x1");
    short i12 = pyi.invoke_extract_attribute<short>(x, "x1");
    long i13 = pyi.invoke_extract_attribute<long>(x, "x1");
    float i21 = pyi.invoke_extract_attribute<float>(x, "x2");
    double i22 = pyi.invoke_extract_attribute<double>(x, "x2");
    std::string i31 = pyi.invoke_extract_attribute<std::string>(x, "x3");
    char* i32 = pyi.invoke_extract_attribute<char*>(x, "x3");
    char i4 = pyi.invoke_extract_attribute<char>(x, "x4");

    EXPECT_EQ(5, i11);
    EXPECT_EQ(5, i12);
    EXPECT_EQ(5, i13);
    EXPECT_EQ(5.5, i21);
    EXPECT_EQ(5.5, i22);
    EXPECT_EQ("hello", i31);
    EXPECT_EQ(0, std::strncmp("hello", i32, 5));
    EXPECT_EQ('c', i4);
 }

TEST_F(SIGRunPythonInvokerTest, invoke_extract_attribute_success_vector)
{
    PythonInvoker pyi;

    Scripting script;

    const std::string rel_path = "res/python_invoker";
    std::vector<std::string> files, classes;

    PluginCollector().collect("/" + rel_path, files);

    std::string base_filename = files[0].substr(files[0].find_last_of("/\\") + 1);
    std::string module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    std::string rpath = rel_path + "/" + base_filename;

    script.load_class_names(classes, files[0]);

    bp::object x = script.si_plugin(module_name, rpath, classes[0]);

    std::vector<int> veci = pyi.invoke_extract_attribute<std::vector<int>>(x, "x5");
    std::vector<std::vector<int>> vecveci = pyi.invoke_extract_attribute<std::vector<std::vector<int>>>(x, "x6");

    EXPECT_EQ(veci.size(), 5);
    EXPECT_EQ(veci[0], 1);
    EXPECT_EQ(veci[1], 2);
    EXPECT_EQ(veci[2], 3);
    EXPECT_EQ(veci[3], 4);
    EXPECT_EQ(veci[4], 5);

    EXPECT_EQ(vecveci.size(), 5);

    for(int i = 0; i < vecveci.size(); i++)
    {
        auto& vec = vecveci[i];

        switch (i)
        {
            case 0:
            {
                EXPECT_EQ(vecveci[i].size(), 5);
                EXPECT_EQ(vec[0], 1);
                EXPECT_EQ(vec[1], 2);
                EXPECT_EQ(vec[2], 3);
                EXPECT_EQ(vec[3], 4);
                EXPECT_EQ(vec[4], 5);
            }
            break;
            case 1:
                EXPECT_EQ(vecveci[i].size(), 3);
                EXPECT_EQ(vec[0], 5);
                EXPECT_EQ(vec[1], 4);
                EXPECT_EQ(vec[2], 3);
            break;
            case 2:
            {
                EXPECT_EQ(vecveci[i].size(), 2);
                EXPECT_EQ(vec[0], 1);
                EXPECT_EQ(vec[1], 2);
            }
            break;
            case 3:
            {
                EXPECT_EQ(vecveci[i].size(), 1);
                EXPECT_EQ(vec[0], 2);
            }
            break;
            case 4:
            {
                EXPECT_EQ(vecveci[i].size(), 4);
                EXPECT_EQ(vec[0], 1);
                EXPECT_EQ(vec[1], 3);
                EXPECT_EQ(vec[2], 5);
                EXPECT_EQ(vec[3], 7);
            }
            break;
        }
    }
}

TEST_F(SIGRunPythonInvokerTest, invoke_extract_attribute_exception_primitives)
{
    PythonInvoker pyi;

    Scripting script;

    const std::string rel_path = "res/python_invoker";
    std::vector<std::string> files, classes;

    PluginCollector().collect("/" + rel_path, files);

    std::string base_filename = files[0].substr(files[0].find_last_of("/\\") + 1);
    std::string module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    std::string rpath = rel_path + "/" + base_filename;

    script.load_class_names(classes, files[0]);

    bp::object x = script.si_plugin(module_name, rpath, classes[0]);

    int i11 = pyi.invoke_extract_attribute<int>(x, "y1");
    short i12 = pyi.invoke_extract_attribute<short>(x, "y1");
    long i13 = pyi.invoke_extract_attribute<long>(x, "y1");
    float i21 = pyi.invoke_extract_attribute<float>(x, "y2");
    double i22 = pyi.invoke_extract_attribute<double>(x, "y2");
    std::string i31 = pyi.invoke_extract_attribute<std::string>(x, "y3");
    char* i32 = pyi.invoke_extract_attribute<char*>(x, "y3");
    char i4 = pyi.invoke_extract_attribute<char>(x, "y4");

    EXPECT_EQ(0, i11);
    EXPECT_EQ(0, i12);
    EXPECT_EQ(0, i13);
    EXPECT_EQ(0, i21);
    EXPECT_EQ(0, i22);
    EXPECT_EQ("", i31);
    EXPECT_EQ(0, std::strncmp("", i32, 0));
    EXPECT_EQ((char) 0, i4);
}

TEST_F(SIGRunPythonInvokerTest, invoke_extract_attribute_exception_vector)
{
    PythonInvoker pyi;

    Scripting script;

    const std::string rel_path = "res/python_invoker";
    std::vector<std::string> files, classes;

    PluginCollector().collect("/" + rel_path, files);

    std::string base_filename = files[0].substr(files[0].find_last_of("/\\") + 1);
    std::string module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    std::string rpath = rel_path + "/" + base_filename;

    script.load_class_names(classes, files[0]);

    bp::object x = script.si_plugin(module_name, rpath, classes[0]);

    std::vector<int> veci = pyi.invoke_extract_attribute<std::vector<int>>(x, "y5");
    std::vector<std::vector<int>> vecveci = pyi.invoke_extract_attribute<std::vector<std::vector<int>>>(x, "y6");

    EXPECT_EQ(veci.size(), 0);
    EXPECT_EQ(vecveci.size(), 0);
}

TEST_F(SIGRunPythonInvokerTest, invoke_set_attribute)
{
//    PythonInvoker pyi;
//    Scripting script;
//
//    const std::string rel_path = "res/python_invoker";
//    std::vector<std::string> files, classes;
//
//    PluginCollector::collect("/" + rel_path, files);
//
//    std::string base_filename = files[0].substr(files[0].find_last_of("/\\") + 1);
//    std::string module_name = base_filename.substr(0, base_filename.find_last_of('.'));
//    std::string rpath = rel_path + "/" + base_filename;
//
//    script.load_class_names(classes, files[0]);
//
//    bp::object x = script.si_plugin(module_name, rpath, classes[0]);
//
//    std::string a = "a";
//    int value = 5;
//
//    pyi.invoke_set_attribute(x, a, value);
//    pyi.invoke_set_attribute(x, "b", std::vector<int>{1, 2, 3, 4, 5}, true);
//    pyi.invoke_set_attribute(x, "c", std::vector<std::vector<int>>{{1, 2}, {3, 4}, {5}}, true);
}

TEST_F(SIGRunPythonInvokerTest, invoke_function_T)
{
    PythonInvoker pyi;
    Scripting script;

    const std::string rel_path = "res/python_invoker";
    std::vector<std::string> files, classes;

    PluginCollector().collect("/" + rel_path, files);

    std::string base_filename = files[0].substr(files[0].find_last_of("/\\") + 1);
    std::string module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    std::string rpath = rel_path + "/" + base_filename;

    script.load_class_names(classes, files[0]);

    bp::object x = script.si_plugin(module_name, rpath, classes[0]);

    int i = pyi.invoke_function<int>(x, "on_enter", x);

    EXPECT_EQ(i, 1);
}