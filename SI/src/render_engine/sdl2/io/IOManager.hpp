

#ifndef SITEST_IOMANAGER_HPP
#define SITEST_IOMANAGER_HPP

#include <vector>
#include <string>

namespace SIRen
{
    class IOManager
    {
    public:
        static bool read_file_to_buffer(const std::string &filepath, std::vector<char> &buffer);
    };
}

#endif //SITEST_IOMANAGER_HPP
