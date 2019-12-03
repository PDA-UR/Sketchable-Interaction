
#ifndef SITEST_ERROR_HPP
#define SITEST_ERROR_HPP

#include <map>
#include <vector>
#include <string>

#define EN 0
#define DE 1

#define ERROR_SIGRUN 100
#define ERROR_IO 200
#define ERROR_PYTHON 300

#define ERROR_UNKNOWN 900

#define ERRORS_EN \
{\
    {ERROR_PYTHON, "An error with a python plugin occured."},\
    {ERROR_IO, "An error with input/output occured."}\
}

#define ERRORS_DE \
{\
    {ERROR_PYTHON, "In einem Python-Plugin ist ein Fehler aufgetreten."},\
    {ERROR_IO, "Bei Input/Output ist ein Fehler aufgetreten."}\
}

int LANGUAGE = 0;

std::map<int, std::map<int, std::string>> ERRORS
{
        {EN, ERRORS_EN},
        {DE, ERRORS_DE}
};

#endif //SITEST_ERROR_HPP
