
#include "PySIMapExposureTest.hpp"
#include <pysi/stl_container_exposure/MapExposure.hpp>
#include <sigrun/util/Util.hpp>
#include <boost/python.hpp>
#include <sigrun/plugin/Scripting.hpp>
#include <boost/shared_ptr.hpp>
#include <glm/glm.hpp>
#include <string>

namespace bp = boost::python;

TEST_F(PySIMapExposureTest, KeyError)
{
    EXPECT_NO_FATAL_FAILURE(KeyError());
}

TEST_F(PySIMapExposureTest, get)
{
    std::map<int32_t, float> map
    {
            {1, 1.0},
            {2, 2.0},
            {3, 3.0},
            {4, 4.0},
            {5, 5.0},
            {6, 6.0}
    };

    float x1 = MapExposure<std::map<int32_t, float>>::get(map, 1);
    float x2 = MapExposure<std::map<int32_t, float>>::get(map, 2);
    float x3 = MapExposure<std::map<int32_t, float>>::get(map, 3);
    float x4 = MapExposure<std::map<int32_t, float>>::get(map, 4);
    float x5 = MapExposure<std::map<int32_t, float>>::get(map, 5);
    float x6 = MapExposure<std::map<int32_t, float>>::get(map, 6);

    ASSERT_EQ(1.0, x1);
    ASSERT_EQ(2.0, x2);
    ASSERT_EQ(3.0, x3);
    ASSERT_EQ(4.0, x4);
    ASSERT_EQ(5.0, x5);
    ASSERT_EQ(6.0, x6);
}

TEST_F(PySIMapExposureTest, set)
{
    std::map<int32_t, float> map
            {
                    {1, 1.0},
                    {2, 2.0},
                    {3, 3.0},
                    {4, 4.0},
                    {5, 5.0},
                    {6, 6.0}
            };

    float x1 = MapExposure<std::map<int32_t, float>>::get(map, 1);
    float x2 = MapExposure<std::map<int32_t, float>>::get(map, 2);
    float x3 = MapExposure<std::map<int32_t, float>>::get(map, 3);
    float x4 = MapExposure<std::map<int32_t, float>>::get(map, 4);
    float x5 = MapExposure<std::map<int32_t, float>>::get(map, 5);
    float x6 = MapExposure<std::map<int32_t, float>>::get(map, 6);

    ASSERT_EQ(1.0, x1);
    ASSERT_EQ(2.0, x2);
    ASSERT_EQ(3.0, x3);
    ASSERT_EQ(4.0, x4);
    ASSERT_EQ(5.0, x5);
    ASSERT_EQ(6.0, x6);

    ASSERT_FALSE(MAP_HAS_KEY(map, 7));


    MapExposure<std::map<int32_t, float>>::set(map, 7, 7.0);
    float x7 = MapExposure<std::map<int32_t, float>>::get(map, 7);
    ASSERT_EQ(7.0, x7);

    ASSERT_TRUE(MAP_HAS_KEY(map, 7));
}

TEST_F(PySIMapExposureTest, del)
{
    std::map<int32_t, float> map
            {
                    {1, 1.0},
                    {2, 2.0},
                    {3, 3.0},
                    {4, 4.0},
                    {5, 5.0},
                    {6, 6.0}
            };

    float x1 = MapExposure<std::map<int32_t, float>>::get(map, 1);
    float x2 = MapExposure<std::map<int32_t, float>>::get(map, 2);
    float x3 = MapExposure<std::map<int32_t, float>>::get(map, 3);
    float x4 = MapExposure<std::map<int32_t, float>>::get(map, 4);
    float x5 = MapExposure<std::map<int32_t, float>>::get(map, 5);
    float x6 = MapExposure<std::map<int32_t, float>>::get(map, 6);

    ASSERT_EQ(1.0, x1);
    ASSERT_EQ(2.0, x2);
    ASSERT_EQ(3.0, x3);
    ASSERT_EQ(4.0, x4);
    ASSERT_EQ(5.0, x5);
    ASSERT_EQ(6.0, x6);

    ASSERT_TRUE(MAP_HAS_KEY(map, 4));

    MapExposure<std::map<int32_t, float>>::del(map, 4);

    ASSERT_FALSE(MAP_HAS_KEY(map, 4));
}

TEST_F(PySIMapExposureTest, in)
{
    std::map<int32_t, float> map
    {
        {1, 1.0},
        {2, 2.0},
        {3, 3.0},
        {4, 4.0},
        {5, 5.0},
        {6, 6.0}
    };

    float x1 = MapExposure<std::map<int32_t, float>>::get(map, 1);
    float x2 = MapExposure<std::map<int32_t, float>>::get(map, 2);
    float x3 = MapExposure<std::map<int32_t, float>>::get(map, 3);
    float x4 = MapExposure<std::map<int32_t, float>>::get(map, 4);
    float x5 = MapExposure<std::map<int32_t, float>>::get(map, 5);
    float x6 = MapExposure<std::map<int32_t, float>>::get(map, 6);

    ASSERT_EQ(1.0, x1);
    ASSERT_EQ(2.0, x2);
    ASSERT_EQ(3.0, x3);
    ASSERT_EQ(4.0, x4);
    ASSERT_EQ(5.0, x5);
    ASSERT_EQ(6.0, x6);

    bool in1 = MapExposure<std::map<int32_t, float>>::in(map, 1);
    bool in2 = MapExposure<std::map<int32_t, float>>::in(map, 2);
    bool in3 = MapExposure<std::map<int32_t, float>>::in(map, 3);
    bool in4 = MapExposure<std::map<int32_t, float>>::in(map, 4);
    bool in5 = MapExposure<std::map<int32_t, float>>::in(map, 5);
    bool in6 = MapExposure<std::map<int32_t, float>>::in(map, 6);
    bool in7 = MapExposure<std::map<int32_t, float>>::in(map, 7);
    bool in8 = MapExposure<std::map<int32_t, float>>::in(map, 8);
    bool in9 = MapExposure<std::map<int32_t, float>>::in(map, 9);
    bool in0 = MapExposure<std::map<int32_t, float>>::in(map, 0);
    bool in11 = MapExposure<std::map<int32_t, float>>::in(map, 11);

    ASSERT_TRUE(in1);
    ASSERT_TRUE(in2);
    ASSERT_TRUE(in3);
    ASSERT_TRUE(in4);
    ASSERT_TRUE(in5);
    ASSERT_TRUE(in6);
    ASSERT_FALSE(in7);
    ASSERT_FALSE(in8);
    ASSERT_FALSE(in9);
    ASSERT_FALSE(in0);
    ASSERT_FALSE(in11);

    MapExposure<std::map<int32_t, float>>::del(map, 4);

    in4 = MapExposure<std::map<int32_t, float>>::in(map, 4);
    ASSERT_FALSE(in4);

    MapExposure<std::map<int32_t, float>>::set(map, 0, 0.0);

    in0 = MapExposure<std::map<int32_t, float>>::in(map, 0);
    ASSERT_TRUE(in0);
    float x0 = MapExposure<std::map<int32_t, float>>::get(map, 0);
    ASSERT_EQ(0.0, x0);
}

TEST_F(PySIMapExposureTest, keys)
{
    Scripting script;

    std::map<int32_t, float> map
    {
        {1, 1.0},
        {2, 2.0},
        {3, 3.0},
        {4, 4.0},
        {5, 5.0},
        {6, 6.0}
    };

    float x1 = MapExposure<std::map<int32_t, float>>::get(map, 1);
    float x2 = MapExposure<std::map<int32_t, float>>::get(map, 2);
    float x3 = MapExposure<std::map<int32_t, float>>::get(map, 3);
    float x4 = MapExposure<std::map<int32_t, float>>::get(map, 4);
    float x5 = MapExposure<std::map<int32_t, float>>::get(map, 5);
    float x6 = MapExposure<std::map<int32_t, float>>::get(map, 6);

    bp::list keys = MapExposure<std::map<int32_t, float>>::keys(map);

    ASSERT_EQ(bp::len(keys), 6);

    for(int32_t i = 0; i < bp::len(keys); ++i)
    {
        int32_t k = bp::extract<int32_t>(keys[i]);
        ASSERT_EQ(i + 1, k);
    }
}

TEST_F(PySIMapExposureTest, values)
{
    Scripting script;

    std::map<int32_t, float> map
            {
                    {1, 1.0},
                    {2, 2.0},
                    {3, 3.0},
                    {4, 4.0},
                    {5, 5.0},
                    {6, 6.0}
            };

    float x1 = MapExposure<std::map<int32_t, float>>::get(map, 1);
    float x2 = MapExposure<std::map<int32_t, float>>::get(map, 2);
    float x3 = MapExposure<std::map<int32_t, float>>::get(map, 3);
    float x4 = MapExposure<std::map<int32_t, float>>::get(map, 4);
    float x5 = MapExposure<std::map<int32_t, float>>::get(map, 5);
    float x6 = MapExposure<std::map<int32_t, float>>::get(map, 6);

    bp::list values = MapExposure<std::map<int32_t, float>>::values(map);

    ASSERT_EQ(bp::len(values), 6);

    for(int32_t i = 0; i < bp::len(values); ++i)
    {
        float k = bp::extract<float>(values[i]);
        ASSERT_EQ((float) (i + 1), k);
    }
}

TEST_F(PySIMapExposureTest, items)
{
    Scripting script;

    std::map<int32_t, float> map
    {
        {1, 1.0},
        {2, 2.0},
        {3, 3.0},
        {4, 4.0},
        {5, 5.0},
        {6, 6.0}
    };

    float x1 = MapExposure<std::map<int32_t, float>>::get(map, 1);
    float x2 = MapExposure<std::map<int32_t, float>>::get(map, 2);
    float x3 = MapExposure<std::map<int32_t, float>>::get(map, 3);
    float x4 = MapExposure<std::map<int32_t, float>>::get(map, 4);
    float x5 = MapExposure<std::map<int32_t, float>>::get(map, 5);
    float x6 = MapExposure<std::map<int32_t, float>>::get(map, 6);

    bp::list items = MapExposure<std::map<int32_t, float>>::items(map);

    ASSERT_EQ(bp::len(items), 6);

    for(int32_t i = 0; i < bp::len(items); ++i)
    {
        const bp::tuple& k = bp::extract<bp::tuple>(items[i]);

        const int32_t& key = bp::extract<int32_t>(k[0]);
        const float& value = bp::extract<float>(k[1]);

        ASSERT_EQ(i + 1, key);
        ASSERT_EQ((float) (i + 1), value);
    }
}

TEST_F(PySIMapExposureTest, MapExposurePartialContour_init)
{
    Scripting script;

    bp::dict d;

    d["test"] = std::vector<glm::vec3> {
        glm::vec3 (1, 1, 1),
        glm::vec3 (2, 2, 1),
        glm::vec3 (3, 3, 1),
        glm::vec3 (4, 4, 1),
        glm::vec3 (5, 5, 1),
        glm::vec3 (6, 6, 1)
    };

    auto mepc = MapExposurePartialContour::init(d);

    ASSERT_TRUE(MAP_HAS_KEY((*mepc), "test"));

    ASSERT_TRUE(VEC_CONTAINS((*mepc)["test"], glm::vec3 (1, 1, 1)));
    ASSERT_TRUE(VEC_CONTAINS((*mepc)["test"], glm::vec3 (2, 2, 1)));
    ASSERT_TRUE(VEC_CONTAINS((*mepc)["test"], glm::vec3 (3, 3, 1)));
    ASSERT_TRUE(VEC_CONTAINS((*mepc)["test"], glm::vec3 (4, 4, 1)));
    ASSERT_TRUE(VEC_CONTAINS((*mepc)["test"], glm::vec3 (5, 5, 1)));
    ASSERT_TRUE(VEC_CONTAINS((*mepc)["test"], glm::vec3 (6, 6, 1)));
    ASSERT_FALSE(VEC_CONTAINS((*mepc)["test"], glm::vec3 (7, 7, 1)));
}

TEST_F(PySIMapExposureTest, MapExposurePartialContour_set)
{
    Scripting script;

    auto mepc = MapExposurePartialContour::init();

    MapExposurePartialContour::set(*mepc, "test", std::vector<glm::vec3>{ glm::vec3(1, 1, 1), glm::vec3(2, 2, 1), glm::vec3(3, 3, 1) });

    ASSERT_TRUE(MAP_HAS_KEY((*mepc), "test"));
    ASSERT_TRUE(VEC_CONTAINS((*mepc)["test"], glm::vec3 (1, 1, 1)));
    ASSERT_TRUE(VEC_CONTAINS((*mepc)["test"], glm::vec3 (2, 2, 1)));
    ASSERT_TRUE(VEC_CONTAINS((*mepc)["test"], glm::vec3 (3, 3, 1)));
    ASSERT_FALSE(VEC_CONTAINS((*mepc)["test"], glm::vec3 (4, 4, 1)));
    ASSERT_FALSE(VEC_CONTAINS((*mepc)["test"], glm::vec3 (5, 5, 1)));
    ASSERT_FALSE(VEC_CONTAINS((*mepc)["test"], glm::vec3 (6, 6, 1)));
    ASSERT_FALSE(VEC_CONTAINS((*mepc)["test"], glm::vec3 (7, 7, 1)));
}

TEST_F(PySIMapExposureTest, MapExposurePartialContour_repr)
{
    Scripting script;

    auto mepc = MapExposurePartialContour::init();

    MapExposurePartialContour::set(*mepc, "test", std::vector<glm::vec3>{ glm::vec3(1, 1, 1), glm::vec3(2, 2, 1), glm::vec3(3, 3, 1) });

    ASSERT_TRUE(MAP_HAS_KEY((*mepc), "test"));
    ASSERT_TRUE(VEC_CONTAINS((*mepc)["test"], glm::vec3 (1, 1, 1)));
    ASSERT_TRUE(VEC_CONTAINS((*mepc)["test"], glm::vec3 (2, 2, 1)));
    ASSERT_TRUE(VEC_CONTAINS((*mepc)["test"], glm::vec3 (3, 3, 1)));
    ASSERT_FALSE(VEC_CONTAINS((*mepc)["test"], glm::vec3 (4, 4, 1)));
    ASSERT_FALSE(VEC_CONTAINS((*mepc)["test"], glm::vec3 (5, 5, 1)));
    ASSERT_FALSE(VEC_CONTAINS((*mepc)["test"], glm::vec3 (6, 6, 1)));
    ASSERT_FALSE(VEC_CONTAINS((*mepc)["test"], glm::vec3 (7, 7, 1)));

    std::string repr = MapExposurePartialContour::repr(*mepc);

    ASSERT_EQ("{, test: [, [1.000000, 1.000000], [2.000000, 2.000000], [3.000000, 3.000000]]}", repr);
}

TEST_F(PySIMapExposureTest, MapExposureString2Function_init)
{
    Scripting script;

    bp::dict d;

    d["test"] = bp::object();

    auto mepc = MapExposureString2Function::init(d);

    ASSERT_TRUE(MAP_HAS_KEY((*mepc), "test"));
}

TEST_F(PySIMapExposureTest, MapExposureString2Function_set)
{
    Scripting script;

    auto mepc = MapExposureString2Function::init();

    MapExposureString2Function::set(*mepc, "test", bp::object());

    ASSERT_TRUE(MAP_HAS_KEY((*mepc), "test"));
}

TEST_F(PySIMapExposureTest, MapExposureString2Function_repr)
{
    Scripting script;

    auto mepc = MapExposureString2Function::init();

    MapExposureString2Function::set(*mepc, "test", bp::object());

    ASSERT_TRUE(MAP_HAS_KEY((*mepc), "test"));

    std::string repr = MapExposureString2Function::repr(*mepc);

    ASSERT_EQ("{, test: function: None, }", repr);
}

TEST_F(PySIMapExposureTest, MapExposureString2_String2FunctionMap_Map_init)
{
    Scripting script;

    bp::dict d, t;

    t["on_enter"] = bp::object();
    t["on_continous"] = bp::object();
    t["on_leave"] = bp::object();
    d["test"] = t;

    auto mepc = MapExposureString2_String2FunctionMap_Map::init(d);

    ASSERT_TRUE(MAP_HAS_KEY((*mepc), "test"));
    ASSERT_TRUE(MAP_HAS_KEY((*mepc)["test"], "on_enter"));
    ASSERT_TRUE(MAP_HAS_KEY((*mepc)["test"], "on_continous"));
    ASSERT_TRUE(MAP_HAS_KEY((*mepc)["test"], "on_leave"));
}

TEST_F(PySIMapExposureTest, MapExposureString2_String2FunctionMap_Map_set)
{
    Scripting script;

    bp::dict t;

    t["on_enter"] = bp::object();
    t["on_continous"] = bp::object();
    t["on_leave"] = bp::object();

    auto mepc = MapExposureString2_String2FunctionMap_Map::init();

    MapExposureString2_String2FunctionMap_Map::set(*mepc, "test", t);

    ASSERT_TRUE(MAP_HAS_KEY((*mepc), "test"));
    ASSERT_TRUE(MAP_HAS_KEY((*mepc)["test"], "on_enter"));
    ASSERT_TRUE(MAP_HAS_KEY((*mepc)["test"], "on_continous"));
    ASSERT_TRUE(MAP_HAS_KEY((*mepc)["test"], "on_leave"));
}

TEST_F(PySIMapExposureTest, MapExposureString2_String2FunctionMap_Map_repr)
{
    Scripting script;

    bp::dict d, t;

    t["on_enter"] = bp::object();
    t["on_continous"] = bp::object();
    t["on_leave"] = bp::object();
    d["test"] = t;

    auto mepc = MapExposureString2_String2FunctionMap_Map::init(d);

    std::string repr = MapExposureString2_String2FunctionMap_Map::repr(*mepc);

    ASSERT_EQ("{, test: {, on_continous: function: None, , on_enter: function: None, , on_leave: function: None, }}", repr);
}
TEST_F(PySIMapExposureTest, create_map)
{
    create_map<MapExposurePartialContour, std::map<std::string, std::vector<glm::vec3>>>("TEST1");
    create_map<MapExposureString2Function, std::map<std::string, bp::object>>("TEST2");
    create_map<MapExposureString2_String2FunctionMap_Map, std::map<std::string, std::map<std::string, bp::object>>>("TEST3");
}
