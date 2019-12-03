

#include "SIGRunErrorTest.hpp"

#include <error/Error.hpp>

TEST_F(SIGRunErrorTest, english)
{
    LANGUAGE = EN;

    ASSERT_EQ("An error with a python plugin occured.", ERRORS[LANGUAGE][ERROR_PYTHON]);
    ASSERT_EQ("An error with input/output occured.", ERRORS[LANGUAGE][ERROR_IO]);
}

TEST_F(SIGRunErrorTest, german)
{
    LANGUAGE = DE;

    ASSERT_EQ("In einem Python-Plugin ist ein Fehler aufgetreten.", ERRORS[LANGUAGE][ERROR_PYTHON]);
    ASSERT_EQ("Bei Input/Output ist ein Fehler aufgetreten.", ERRORS[LANGUAGE][ERROR_IO]);
}