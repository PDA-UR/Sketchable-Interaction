

#include "SIGRunPrintTest.hpp"
#include "../../../../src/debug/Print.hpp"

TEST_F(SIGRunPrintTest, primitives)
{
    EXPECT_NO_FATAL_FAILURE(Print::print((int) 1));
    EXPECT_NO_FATAL_FAILURE(Print::print((short) 1));
    EXPECT_NO_FATAL_FAILURE(Print::print((long) 1));
    EXPECT_NO_FATAL_FAILURE(Print::print((signed) 1));
    EXPECT_NO_FATAL_FAILURE(Print::print((unsigned) 1));
    EXPECT_NO_FATAL_FAILURE(Print::print((size_t) 1));
    EXPECT_NO_FATAL_FAILURE(Print::print((float) 1));
    EXPECT_NO_FATAL_FAILURE(Print::print((double) 1));
    EXPECT_NO_FATAL_FAILURE(Print::print((u_long) 1));
    EXPECT_NO_FATAL_FAILURE(Print::print((u_int8_t) 1));
    EXPECT_NO_FATAL_FAILURE(Print::print((u_int16_t) 1));
    EXPECT_NO_FATAL_FAILURE(Print::print((u_int32_t) 1));
    EXPECT_NO_FATAL_FAILURE(Print::print((u_int64_t) 1));
    EXPECT_NO_FATAL_FAILURE(Print::print((char) 1));
    EXPECT_NO_FATAL_FAILURE(Print::print((u_char) 1));
    EXPECT_NO_FATAL_FAILURE(Print::print((u_quad_t) 1));
    EXPECT_NO_FATAL_FAILURE(Print::print(std::string("Hello")));
    EXPECT_NO_FATAL_FAILURE(Print::print((char *) "Hello"));
}

TEST_F(SIGRunPrintTest, python_object)
{
    std::vector<std::string> list = {"Hello", "World", "Test", "Case"};

    EXPECT_NO_FATAL_FAILURE(Print::print(list));
}

TEST_F(SIGRunPrintTest, vector_of_vectors)
{
    std::vector<std::vector<std::string>> list =
            {
                    {"Hello", "World", "Test", "Case"},
                    {"World", "Test", "Case", "Hello"},
                    {"Test", "Case", "Hello", "World"},
                    {"Case", "Hello", "World", "Test"}
            };

    EXPECT_NO_FATAL_FAILURE(Print::print(list));
}

TEST_F(SIGRunPrintTest, map)
{
    std::map<int32_t, int32_t> map2;

    for(uint32_t i = 0; i < 5; i++)
        map2.insert(std::make_pair<int32_t, int32_t>(reinterpret_cast<int32_t &&>(i), reinterpret_cast<int32_t &&>(i)));

    EXPECT_NO_FATAL_FAILURE(Print::print(map2));
}

TEST_F(SIGRunPrintTest, chain)
{
    std::map<int32_t, int32_t> map2;

    for(uint32_t i = 0; i < 5; i++)
        map2.insert(std::make_pair<int, int>(reinterpret_cast<int32_t &&>(i), reinterpret_cast<int32_t &&>(i)));

    std::vector<std::vector<std::string>> list =
            {
                    {"Hello", "World", "Test", "Case"},
                    {"World", "Test", "Case", "Hello"},
                    {"Test", "Case", "Hello", "World"},
                    {"Case", "Hello", "World", "Test"}
            };

    EXPECT_NO_FATAL_FAILURE(Print::print(1, map2, list));
}