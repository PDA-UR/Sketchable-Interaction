#include <iostream>
#include "Test.h"

int main()
{
    Test t;
    t.hello();

    std::cout << Test::i << std::endl;
    std::cout << t.i << std::endl;

    t.i += 1337;

    std::cout << Test::i << std::endl;
    std::cout << t.i << std::endl;

    return 0;
}