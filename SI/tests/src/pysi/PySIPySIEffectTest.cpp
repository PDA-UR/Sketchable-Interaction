

#include "PySIPySIEffectTest.hpp"
#include <boost/python.hpp>
#include <sigrun/plugin/Scripting.hpp>
#include <sigrun/plugin/PluginCollector.hpp>

TEST_F(PySIPySIEffectTest, PySIEffect_Python_Side)
{
    char buf[FILENAME_MAX];
    getcwd(buf, FILENAME_MAX);
    std::string directory(buf);

    bp::import("sys").attr("path").attr("insert")(0, directory + "/tests/src/pysi");

    bp::object o = bp::import("PySIPySIEffectTest");
    o.attr(SI_INTERNAL_NAME) = "PySIPySIEffectTest";

    std::vector<glm::vec3> contour
    {
        glm::vec3(1, 1, 1),
        glm::vec3(1, 5, 1),
        glm::vec3(5, 5, 1),
        glm::vec3(5, 1, 1)
    };

    ASSERT_TRUE(bp::extract<bool>(o.attr("PySIPySIEffectTest")().attr("start")()));
}

TEST_F(PySIPySIEffectTest, init)
{
    std::vector<glm::vec3> contour
    {
        glm::vec3(1, 1, 1),
        glm::vec3(1, 5, 1),
        glm::vec3(5, 5, 1),
        glm::vec3(5, 1, 1)
    };

    EXPECT_NO_FATAL_FAILURE(PySIEffect pysie(contour, _UUID_, "", bp::dict()));
}

TEST_F(PySIPySIEffectTest, add_data)
{
    std::vector<glm::vec3> contour
    {
        glm::vec3(1, 1, 1),
        glm::vec3(1, 5, 1),
        glm::vec3(5, 5, 1),
        glm::vec3(5, 1, 1)
    };

    PySIEffect pysie(contour, _UUID_, "", bp::dict());

    EXPECT_NO_FATAL_FAILURE(pysie.__set_data__("test", bp::object(1), SI_DATA_TYPE_INT, bp::dict()));
    EXPECT_NO_FATAL_FAILURE(pysie.__set_data__("test2", bp::object(1.0), SI_DATA_TYPE_FLOAT, bp::dict()));
    EXPECT_NO_FATAL_FAILURE(pysie.__set_data__("test3", bp::object(std::string("test")), SI_DATA_TYPE_STRING, bp::dict()));
    EXPECT_NO_FATAL_FAILURE(pysie.__set_data__("test4", bp::object(true), SI_DATA_TYPE_BOOL, bp::dict()));

    ASSERT_TRUE(pysie.d_data_changed);
}

TEST_F(PySIPySIEffectTest, signal_deletion)
{
    std::vector<glm::vec3> contour
    {
        glm::vec3(1, 1, 1),
        glm::vec3(1, 5, 1),
        glm::vec3(5, 5, 1),
        glm::vec3(5, 1, 1)
    };

    PySIEffect pysie(contour, _UUID_, "", bp::dict());

    EXPECT_NO_FATAL_FAILURE(pysie.__signal_deletion__());
}

TEST_F(PySIPySIEffectTest, embed_file_standard_appliation_into_context)
{
    std::vector<glm::vec3> contour
    {
        glm::vec3(1, 1, 1),
        glm::vec3(1, 5, 1),
        glm::vec3(5, 5, 1),
        glm::vec3(5, 1, 1)
    };

    PySIEffect pysie(contour, _UUID_, "", bp::dict());

    EXPECT_NO_FATAL_FAILURE(pysie.__embed_file_standard_appliation_into_context__(std::string("uuid"), std::string("path")));
}

TEST_F(PySIPySIEffectTest, destroy_embedded_window)
{
    std::vector<glm::vec3> contour
    {
        glm::vec3(1, 1, 1),
        glm::vec3(1, 5, 1),
        glm::vec3(5, 5, 1),
        glm::vec3(5, 1, 1)
    };

    PySIEffect pysie(contour, _UUID_, "", bp::dict());

    EXPECT_NO_FATAL_FAILURE(pysie.__destroy_embedded_file_standard_appliation_in_context__(std::string("uuid")));
}
