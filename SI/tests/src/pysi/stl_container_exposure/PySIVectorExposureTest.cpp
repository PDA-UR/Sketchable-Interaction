
#include "PySIVectorExposureTest.hpp"
#include <boost/python.hpp>
#include <sigrun/plugin/Scripting.hpp>
#include <boost/shared_ptr.hpp>
#include <glm/glm.hpp>
#include <string>
#include <sigrun/util/Util.hpp>
#include <pysi/stl_container_exposure/VectorExposure.hpp>

namespace bp = boost::python;

TEST_F(PySIVectorExposureTest, IndexError)
{
    EXPECT_NO_FATAL_FAILURE(IndexError());
}

TEST_F(PySIVectorExposureTest, get)
{
    std::vector<int32_t> test
    {
        1, 2, 3, 4, 5, 6, 7
    };

    int32_t value0 = VectorExposure<std::vector<int32_t>>::get(test, 0);
    int32_t value1 = VectorExposure<std::vector<int32_t>>::get(test, 1);
    int32_t value2 = VectorExposure<std::vector<int32_t>>::get(test, 2);
    int32_t value3 = VectorExposure<std::vector<int32_t>>::get(test, 3);
    int32_t value4 = VectorExposure<std::vector<int32_t>>::get(test, 4);
    int32_t value5 = VectorExposure<std::vector<int32_t>>::get(test, 5);
    int32_t value6 = VectorExposure<std::vector<int32_t>>::get(test, 6);

    ASSERT_EQ(value0, 1);
    ASSERT_EQ(value1, 2);
    ASSERT_EQ(value2, 3);
    ASSERT_EQ(value3, 4);
    ASSERT_EQ(value4, 5);
    ASSERT_EQ(value5, 6);
    ASSERT_EQ(value6, 7);
}

TEST_F(PySIVectorExposureTest, get_slice)
{
    Scripting script;

    std::vector<int32_t> test
    {
        1, 2, 3, 4, 5, 6, 7
    };

    int32_t value0 = VectorExposure<std::vector<int32_t>>::get(test, 0);
    int32_t value1 = VectorExposure<std::vector<int32_t>>::get(test, 1);
    int32_t value2 = VectorExposure<std::vector<int32_t>>::get(test, 2);
    int32_t value3 = VectorExposure<std::vector<int32_t>>::get(test, 3);
    int32_t value4 = VectorExposure<std::vector<int32_t>>::get(test, 4);
    int32_t value5 = VectorExposure<std::vector<int32_t>>::get(test, 5);
    int32_t value6 = VectorExposure<std::vector<int32_t>>::get(test, 6);

    ASSERT_EQ(value0, 1);
    ASSERT_EQ(value1, 2);
    ASSERT_EQ(value2, 3);
    ASSERT_EQ(value3, 4);
    ASSERT_EQ(value4, 5);
    ASSERT_EQ(value5, 6);
    ASSERT_EQ(value6, 7);

    bp::slice slice(0, 2);

    std::vector<int32_t> sliced = VectorExposure<std::vector<int32_t>>::get_slice(test, slice);

    ASSERT_EQ(2, sliced.size());

    ASSERT_EQ(sliced[0], 1);
    ASSERT_EQ(sliced[1], 2);

    slice = bp::slice (0, 7, 2);

    sliced = VectorExposure<std::vector<int32_t>>::get_slice(test, slice);

    ASSERT_EQ(4, sliced.size());

    ASSERT_EQ(sliced[0], 1);
    ASSERT_EQ(sliced[1], 3);
    ASSERT_EQ(sliced[2], 5);
    ASSERT_EQ(sliced[3], 7);

    slice = bp::slice (2, 6, 3);

    sliced = VectorExposure<std::vector<int32_t>>::get_slice(test, slice);

    ASSERT_EQ(2, sliced.size());

    ASSERT_EQ(sliced[0], 3);
    ASSERT_EQ(sliced[1], 6);
}

TEST_F(PySIVectorExposureTest, set)
{
    Scripting script;

    std::vector<int32_t> test
    {
        1, 2, 3, 4, 5, 6, 7
    };

    ASSERT_EQ(test[2], 3);
    VectorExposure<std::vector<int32_t>>::set(test, 2, 1337);

    ASSERT_EQ(test[2], 1337);
}

TEST_F(PySIVectorExposureTest, del)
{
    Scripting script;

    std::vector<int32_t> test
    {
        1, 2, 3, 4, 5, 6, 7
    };

    ASSERT_EQ(test.size(), 7);

    ASSERT_EQ(test[2], 3);
    VectorExposure<std::vector<int32_t>>::del(test, 2);

    ASSERT_EQ(test[2], 4);

    ASSERT_EQ(test.size(), 6);
}

TEST_F(PySIVectorExposureTest, add)
{
    Scripting script;

    std::vector<int32_t> test
    {
        1, 2, 3, 4, 5, 6, 7
    };

    ASSERT_EQ(test.size(), 7);

    VectorExposure<std::vector<int32_t>>::add(test, 8);

    ASSERT_EQ(test[7], 8);

    ASSERT_EQ(test.size(), 8);
}

TEST_F(PySIVectorExposureTest, in)
{
    Scripting script;

    std::vector<int32_t> test
    {
        1, 2, 3, 4, 5, 6, 7
    };

    ASSERT_EQ(test.size(), 7);

    bool in1 = VectorExposure<std::vector<int32_t>>::in(test, 1);
    bool in2 = VectorExposure<std::vector<int32_t>>::in(test, 2);
    bool in3 = VectorExposure<std::vector<int32_t>>::in(test, 3);
    bool in4 = VectorExposure<std::vector<int32_t>>::in(test, 4);
    bool in5 = VectorExposure<std::vector<int32_t>>::in(test, 5);
    bool in6 = VectorExposure<std::vector<int32_t>>::in(test, 6);
    bool in7 = VectorExposure<std::vector<int32_t>>::in(test, 7);
    bool in8 = VectorExposure<std::vector<int32_t>>::in(test, 8);

    ASSERT_TRUE(in1);
    ASSERT_TRUE(in2);
    ASSERT_TRUE(in3);
    ASSERT_TRUE(in4);
    ASSERT_TRUE(in5);
    ASSERT_TRUE(in6);
    ASSERT_TRUE(in7);
    ASSERT_FALSE(in8);
}

TEST_F(PySIVectorExposureTest, index)
{
    Scripting script;

    std::vector<int32_t> test
    {
        1, 2, 3, 4, 5, 6, 7
    };

    ASSERT_EQ(test.size(), 7);

    int32_t index1 = VectorExposure<std::vector<int32_t>>::index(test, 1);
    int32_t index2 = VectorExposure<std::vector<int32_t>>::index(test, 2);
    int32_t index3 = VectorExposure<std::vector<int32_t>>::index(test, 3);
    int32_t index4 = VectorExposure<std::vector<int32_t>>::index(test, 4);
    int32_t index5 = VectorExposure<std::vector<int32_t>>::index(test, 5);
    int32_t index6 = VectorExposure<std::vector<int32_t>>::index(test, 6);
    int32_t index7 = VectorExposure<std::vector<int32_t>>::index(test, 7);
    int32_t index8 = VectorExposure<std::vector<int32_t>>::index(test, 8);

    ASSERT_EQ(index1, 0);
    ASSERT_EQ(index2, 1);
    ASSERT_EQ(index3, 2);
    ASSERT_EQ(index4, 3);
    ASSERT_EQ(index5, 4);
    ASSERT_EQ(index6, 5);
    ASSERT_EQ(index7, 6);
    ASSERT_EQ(index8, -1);
}

TEST_F(PySIVectorExposureTest, VectorExposureVec3_init)
{
    Scripting script;

    std::vector<glm::vec3> vec
    {
        glm::vec3 (1, 1, 1),
        glm::vec3 (2, 2, 1),
        glm::vec3 (3, 3, 1),
        glm::vec3 (4, 4, 1),
        glm::vec3 (5, 5, 1),
        glm::vec3 (6, 6, 1)
    };

    bp::list d, t;

    for(auto& p: vec)
    {
        t = bp::list();

        t.append(p.x);
        t.append(p.y);
        d.append(t);
    }

    auto vev = VectorExposureVec3::init(d);

    ASSERT_EQ(vev->size(), vec.size());

    for(int32_t i = 0; i < vec.size(); ++i)
    {
        ASSERT_EQ(vec[i].x, (*vev)[i].x);
        ASSERT_EQ(vec[i].y, (*vev)[i].y);
    }
}

TEST_F(PySIVectorExposureTest, VectorExposureVec3_add)
{
    Scripting script;

    auto vev = VectorExposureVec3::init();

    bp::list in;
    in.append(1);
    in.append(1);

    ASSERT_EQ(vev->size(), 0);

    VectorExposureVec3::add(*vev, in);

    ASSERT_EQ(vev->size(), 1);

    ASSERT_EQ((*vev)[0].x , 1);
    ASSERT_EQ((*vev)[0].y , 1);
}

TEST_F(PySIVectorExposureTest, VectorExposureVec3_set)
{
    Scripting script;

    std::vector<glm::vec3> vec
    {
        glm::vec3 (1, 1, 1),
        glm::vec3 (2, 2, 1),
        glm::vec3 (3, 3, 1),
        glm::vec3 (4, 4, 1),
        glm::vec3 (5, 5, 1),
        glm::vec3 (6, 6, 1)
    };

    bp::list d, t, u;

    for(auto& p: vec)
    {
        t = bp::list();

        t.append(p.x);
        t.append(p.y);
        d.append(t);
    }

    auto vev = VectorExposureVec3::init(d);

    u.append(1337);
    u.append(1337);

    ASSERT_EQ((*vev)[1], glm::vec3(2, 2, 1));

    VectorExposureVec3::set(*vev, 1, u);

    ASSERT_EQ((*vev)[1], glm::vec3(1337, 1337, 1));
}

TEST_F(PySIVectorExposureTest, VectorExposureVec3_repr)
{
    Scripting script;

    std::vector<glm::vec3> vec
            {
                    glm::vec3 (1, 1, 1),
                    glm::vec3 (2, 2, 1),
                    glm::vec3 (3, 3, 1),
                    glm::vec3 (4, 4, 1),
                    glm::vec3 (5, 5, 1),
                    glm::vec3 (6, 6, 1)
            };

    bp::list d, t, u;

    for(auto& p: vec)
    {
        t = bp::list();

        t.append(p.x);
        t.append(p.y);
        d.append(t);
    }

    auto vev = VectorExposureVec3::init(d);

    std::string repr = VectorExposureVec3::repr(*vev);

    ASSERT_EQ("[, [1.000000, 1.000000], [2.000000, 2.000000], [3.000000, 3.000000], [4.000000, 4.000000], [5.000000, 5.000000], [6.000000, 6.000000]]", repr);
}

TEST_F(PySIVectorExposureTest, VectorExposureString_init)
{
    Scripting script;

    std::vector<std::string> vec
    {
        "test1", "test2", "test3", "test4", "test5", "test6"
    };

    bp::list d;

    for(auto& p: vec)
        d.append(p);

    auto vev = VectorExposureString::init(d);

    ASSERT_EQ(vev->size(), vec.size());

    for(int32_t i = 0; i < vec.size(); ++i)
        ASSERT_EQ((*vev)[i], vec[i]);
}

TEST_F(PySIVectorExposureTest, VectorExposureString_add)
{
    Scripting script;

    auto vev = VectorExposureString::init();
    ASSERT_EQ(vev->size(), 0);

    VectorExposureString::add(*vev, std::string("test"));

    ASSERT_EQ(vev->size(), 1);
    ASSERT_EQ((*vev)[0] , std::string("test"));
}

TEST_F(PySIVectorExposureTest, VectorExposureString_set)
{
    Scripting script;

    std::vector<std::string> vec
    {
        "test1", "test2", "test3", "test4", "test5", "test6"
    };

    bp::list d;

    for(auto& p: vec)
        d.append(p);

    auto vev = VectorExposureString::init(d);

    ASSERT_EQ((*vev)[1], "test2");

    VectorExposureString::set(*vev, 1, "test1337");

    ASSERT_EQ((*vev)[1], "test1337");
}

TEST_F(PySIVectorExposureTest, VectorExposureString_repr)
{
    Scripting script;

    std::vector<std::string> vec
            {
                    "test1", "test2", "test3", "test4", "test5", "test6"
            };

    bp::list d;

    for(auto& p: vec)
        d.append(p);

    auto vev = VectorExposureString::init(d);

    std::string repr = VectorExposureString::repr(*vev);

    ASSERT_EQ("[, test1, test2, test3, test4, test5, test6]", repr);
}

TEST_F(PySIVectorExposureTest, VectorExposureLinkRelation_init)
{
    Scripting script;

    bp::list d, l0, l1;
    l0.append("sender0");
    l0.append("attrib_sender0");
    l0.append("recv0");
    l0.append("attrib_recv0");

    l1.append("sender1");
    l1.append("attrib_sender1");
    l1.append("recv1");
    l1.append("attrib_recv1");

    d.append(l0);
    d.append(l1);

    auto vev = VectorExposureLinkRelation::init(d);

    ASSERT_EQ(vev->size(), 2);

    for(int32_t i = 0; i < vev->size(); ++i)
    {
        ASSERT_EQ((*vev)[i].sender, "sender" + std::to_string(i));
        ASSERT_EQ((*vev)[i].sender_attrib, "attrib_sender" + std::to_string(i));
        ASSERT_EQ((*vev)[i].recv, "recv" + std::to_string(i));
        ASSERT_EQ((*vev)[i].recv_attrib, "attrib_recv" + std::to_string(i));
    }
}

TEST_F(PySIVectorExposureTest, VectorExposureLinkRelation_add)
{
    Scripting script;

    bp::list d;
    d.append("sender0");
    d.append("attrib_sender0");
    d.append("recv0");
    d.append("attrib_recv0");

    auto vev = VectorExposureLinkRelation::init();
    ASSERT_EQ(vev->size(), 0);

    VectorExposureLinkRelation::add(*vev, d);

    ASSERT_EQ(vev->size(), 1);
    ASSERT_EQ((*vev)[0].sender, "sender" + std::to_string(0));
    ASSERT_EQ((*vev)[0].sender_attrib, "attrib_sender" + std::to_string(0));
    ASSERT_EQ((*vev)[0].recv, "recv" + std::to_string(0));
    ASSERT_EQ((*vev)[0].recv_attrib, "attrib_recv" + std::to_string(0));
}

TEST_F(PySIVectorExposureTest, VectorExposureLinkRelation_set)
{
    Scripting script;

    bp::list d, l0, l1, l2;
    l0.append("sender0");
    l0.append("attrib_sender0");
    l0.append("recv0");
    l0.append("attrib_recv0");

    l1.append("sender1");
    l1.append("attrib_sender1");
    l1.append("recv1");
    l1.append("attrib_recv1");

    l2.append("sender2");
    l2.append("attrib_sender2");
    l2.append("recv2");
    l2.append("attrib_recv2");

    d.append(l0);
    d.append(l1);

    auto vev = VectorExposureLinkRelation::init(d);

    ASSERT_EQ(vev->size(), 2);


    ASSERT_EQ((*vev)[1].sender, "sender" + std::to_string(1));
    ASSERT_EQ((*vev)[1].sender_attrib, "attrib_sender" + std::to_string(1));
    ASSERT_EQ((*vev)[1].recv, "recv" + std::to_string(1));
    ASSERT_EQ((*vev)[1].recv_attrib, "attrib_recv" + std::to_string(1));

    VectorExposureLinkRelation::set(*vev, 1, l2);

    ASSERT_EQ((*vev)[1].sender, "sender" + std::to_string(2));
    ASSERT_EQ((*vev)[1].sender_attrib, "attrib_sender" + std::to_string(2));
    ASSERT_EQ((*vev)[1].recv, "recv" + std::to_string(2));
    ASSERT_EQ((*vev)[1].recv_attrib, "attrib_recv" + std::to_string(2));
}

TEST_F(PySIVectorExposureTest, VectorExposureLinkRelation_repr)
{
    Scripting script;

    bp::list d, l0, l1;
    l0.append("sender0");
    l0.append("attrib_sender0");
    l0.append("recv0");
    l0.append("attrib_recv0");

    l1.append("sender1");
    l1.append("attrib_sender1");
    l1.append("recv1");
    l1.append("attrib_recv1");

    d.append(l0);
    d.append(l1);

    auto vev = VectorExposureLinkRelation::init(d);

    std::string repr = VectorExposureLinkRelation::repr(*vev);

    ASSERT_EQ("[, [sender0, attrib_sender0, recv0, attrib_recv0], [sender1, attrib_sender1, recv1, attrib_recv1]]", repr);
}

TEST_F(PySIVectorExposureTest, create_vector)
{
    Scripting script;

    create_vector<VectorExposureVec3, std::vector<glm::vec3>>("TEST1");
    create_vector<VectorExposureLinkRelation, std::vector<LinkCandidate>>("TEST2");
    create_vector<VectorExposureString, std::vector<std::string>>("TEST3");
}