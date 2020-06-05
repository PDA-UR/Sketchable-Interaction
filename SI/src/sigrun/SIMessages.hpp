
#ifndef SITEST_SIMESSAGES_HPP
#define SITEST_SIMESSAGES_HPP

#include <map>
#include <vector>
#include <string>

#define EN 0
#define DE 1

#define ERROR_SIGRUN 100
#define ERROR_IO 200
#define ERROR_PYTHON 300

#define ERROR_UNKNOWN 900

#define MESSAGES_EN \
{\
    {ERROR_PYTHON, "An error with a python plugin occured."},\
    {ERROR_IO, "An error with input/output occured."}\
}

#define MESSAGES_DE \
{\
    {ERROR_PYTHON, "In einem Python-Plugin ist ein Fehler aufgetreten."},\
    {ERROR_IO, "Bei Input/Output ist ein Fehler aufgetreten."}\
}

uint64_t __SI_SIGRUN_LANGUAGE__ = 0;

std::map<int, std::map<int, std::string>> MESSAGES
{
        {EN, MESSAGES_EN},
        {DE, MESSAGES_DE}
};

#endif //SITEST_SIMESSAGES_HPP
