

#ifndef SITEST_SIRENPRINT_HPP
#define SITEST_SIRENPRINT_HPP


#include <mutex>
#include <iostream>
#include <sstream>

// Thread Safe (might be included in core runtime as well

class SIRenPrint : public std::ostringstream
{
public:
    SIRenPrint() = default;

    ~SIRenPrint()
    {
        std::lock_guard<std::mutex> guard(_mutexPrint);
        std::cout << this->str();
    }

    template <typename T>
    static void print(const T& arg)
    {
        SIRenPrint{} << arg << std::endl;
    }

private:
    static std::mutex _mutexPrint;
};


#endif //SITEST_SIRENPRINT_HPP
