

#include "include/SiPrintTest.hpp"
#include <debug/Print.hpp>

TEST_F(SiPrintTest, python_object)
{
    bp::object obj;

    EXPECT_NO_FATAL_FAILURE(Print::print(obj));
}