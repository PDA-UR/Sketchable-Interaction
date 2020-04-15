

#include "PySIPySIEffectTest.hpp"
#include <QProcess>
#include <boost/python.hpp>
#include <sigrun/plugin/Scripting.hpp>
#include <sigrun/plugin/PythonInvoker.hpp>
#include <sigrun/plugin/PluginCollector.hpp>

TEST_F(PySIPySIEffectTest, PySIEffect_Python_Side)
{
    Scripting script;

    std::string module = "PySIPySIEffectTest";
    std::string path = "src/pysi/PySIPySIEffectTest.py";
    std::string clazz = "PySIPySIEffectTest";

    ASSERT_TRUE(bp::extract<bool>(script.si_plugin(module, path, clazz).attr("start")()));
}

TEST_F(PySIPySIEffectTest, init)
{

}
