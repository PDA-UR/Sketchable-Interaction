

#include "include/SiPrintTest.hpp"
#include <core/debug/Print.hpp>
#include <map>

TEST_F(SiPrintTest, python_object)
{
    bp::object obj;

    EXPECT_NO_FATAL_FAILURE(SI::Print::print(obj));
}

TEST_F(SiPrintTest, primitives)
{
    EXPECT_NO_FATAL_FAILURE(SI::Print::print((int) 1));
    EXPECT_NO_FATAL_FAILURE(SI::Print::print((short) 1));
    EXPECT_NO_FATAL_FAILURE(SI::Print::print((long) 1));
    EXPECT_NO_FATAL_FAILURE(SI::Print::print((signed) 1));
    EXPECT_NO_FATAL_FAILURE(SI::Print::print((unsigned) 1));
    EXPECT_NO_FATAL_FAILURE(SI::Print::print((size_t) 1));
    EXPECT_NO_FATAL_FAILURE(SI::Print::print((float) 1));
    EXPECT_NO_FATAL_FAILURE(SI::Print::print((double) 1));
    EXPECT_NO_FATAL_FAILURE(SI::Print::print((u_long) 1));
    EXPECT_NO_FATAL_FAILURE(SI::Print::print((u_int8_t) 1));
    EXPECT_NO_FATAL_FAILURE(SI::Print::print((u_int16_t) 1));
    EXPECT_NO_FATAL_FAILURE(SI::Print::print((u_int32_t) 1));
    EXPECT_NO_FATAL_FAILURE(SI::Print::print((char) 1));
    EXPECT_NO_FATAL_FAILURE(SI::Print::print((u_char) 1));
    EXPECT_NO_FATAL_FAILURE(SI::Print::print((u_quad_t) 1));
    EXPECT_NO_FATAL_FAILURE(SI::Print::print(std::string("Hello")));
    EXPECT_NO_FATAL_FAILURE(SI::Print::print((char *) "Hello"));
}

TEST_F(SiPrintTest, vector)
{
    std::vector<std::string> list = {"Hello", "World", "Test", "Case"};

    EXPECT_NO_FATAL_FAILURE(SI::Print::print(list));
}

TEST_F(SiPrintTest, vector_of_vectors)
{
    std::vector<std::vector<std::string>> list =
    {
            {"Hello", "World", "Test", "Case"},
            {"World", "Test", "Case", "Hello"},
            {"Test", "Case", "Hello", "World"},
            {"Case", "Hello", "World", "Test"}
    };

    EXPECT_NO_FATAL_FAILURE(SI::Print::print(list));
}

TEST_F(SiPrintTest, map)
{
    std::map<int, SI::Layer*> map;
    std::map<int, int> map2;

    for(int i = 0; i < 5; i++)
    {
        map.insert(std::make_pair<int, SI::Layer*>(reinterpret_cast<int &&>(i), new SI::Layer(i)));
        map2.insert(std::make_pair<int, int>(reinterpret_cast<int &&>(i), reinterpret_cast<int &&>(i)));
    }

    EXPECT_NO_FATAL_FAILURE(SI::Print::print(map));
    EXPECT_NO_FATAL_FAILURE(SI::Print::print(map2));
}
