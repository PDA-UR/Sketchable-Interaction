
#include <fstream>
#include <string>
#include "IOManager.hpp"

bool IOManager::read_file_to_buffer(const std::string &filepath, std::vector<char> &buffer)
{
    std::ifstream file(filepath, std::ios::binary);

    if (file.fail())
        return false;

    file.seekg(0, std::ios::end);
    int file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    file_size -= file.tellg();

    buffer.resize(file_size);
    file.read(&buffer[0], file_size);
    file.close();

    return true;
}
