
#include <iostream>

class Test
{
    public:
    Test() {}
    ~Test() = default;

    static int i;

    void hello()
    {
        std::cout << "Hello" << std::endl;
    }

    void print_i()
    {
        std::cout << i << std::endl;
    }
};