

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
    PySIEffect pysie;

    std::vector<glm::vec3> contour {
        glm::vec3(1, 1, 1),
        glm::vec3(1, 5, 1),
        glm::vec3(5, 5, 1),
        glm::vec3(5, 1, 1),
    };

    EXPECT_NO_FATAL_FAILURE(pysie.init(contour, contour, _UUID_, bp::dict()));
}

TEST_F(PySIPySIEffectTest, add_data)
{
    PySIEffect pysie;

    EXPECT_NO_FATAL_FAILURE(pysie.__add_data__("test", bp::object(1), SI_DATA_TYPE_INT));
    EXPECT_NO_FATAL_FAILURE(pysie.__add_data__("test2", bp::object(1.0), SI_DATA_TYPE_FLOAT));
    EXPECT_NO_FATAL_FAILURE(pysie.__add_data__("test3", bp::object(std::string("test")), SI_DATA_TYPE_STRING));
    EXPECT_NO_FATAL_FAILURE(pysie.__add_data__("test4", bp::object(true), SI_DATA_TYPE_BOOL));

    ASSERT_TRUE(pysie.d_data_changed);
}

TEST_F(PySIPySIEffectTest, notify_shape_changed)
{
    PySIEffect pysie;
    EXPECT_NO_FATAL_FAILURE(pysie.notify_shape_changed());

    ASSERT_TRUE(pysie.d_has_shape_changed);
    ASSERT_FALSE(pysie.d_require_resample);

    pysie.d_has_shape_changed = false;
    EXPECT_NO_FATAL_FAILURE(pysie.notify_shape_changed(true));

    ASSERT_TRUE(pysie.d_has_shape_changed);
    ASSERT_TRUE(pysie.d_require_resample);
}

TEST_F(PySIPySIEffectTest, signal_deletion)
{
    PySIEffect pysie;

    EXPECT_NO_FATAL_FAILURE(pysie.signal_deletion());

}

TEST_F(PySIPySIEffectTest, show_folder_contents_page)
{
    std::vector<std::string> strs {"test1", "test2"};
    std::string uuid = _UUID_;


    PySIEffect pysie;
    EXPECT_NO_FATAL_FAILURE(pysie.__show_folder_contents__(strs, uuid, false));
    EXPECT_NO_FATAL_FAILURE(pysie.__show_folder_contents__(strs, uuid, true));
}

TEST_F(PySIPySIEffectTest, embed_file_standard_appliation_into_context)
{
    PySIEffect pysie;
    EXPECT_NO_FATAL_FAILURE(pysie.__embed_file_standard_appliation_into_context__(std::string("uuid"), std::string("path")));
}

TEST_F(PySIPySIEffectTest, destroy_embedded_window)
{
    PySIEffect pysie;
    EXPECT_NO_FATAL_FAILURE(pysie.__destroy_embedded_file_standard_appliation_in_context__(std::string("uuid")));
}
