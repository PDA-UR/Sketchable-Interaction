

#include "PySIPySIEffectTest.hpp"
#include <QProcess>
#include <boost/python.hpp>
#include <sigrun/plugin/Scripting.hpp>
#include <sigrun/plugin/PythonInvoker.hpp>
#include <sigrun/plugin/PluginCollector.hpp>
#include <debug/Print.hpp>

TEST_F(PySIPySIEffectTest, PySIEffect_Python_Side)
{
    Scripting script;

    std::string module = "PySIPySIEffectTest";
    std::string path = "src/pysi/PySIPySIEffectTest.py";
    std::string clazz = "PySIPySIEffectTest";

    HANDLE_PYTHON_CALL(
        bp::object o = script.si_plugin(module, path, clazz);
        bool python_test_success =  bp::extract<bool>(o.attr("start")());

        ASSERT_TRUE(python_test_success);
    )
}

TEST_F(PySIPySIEffectTest, init)
{

}
