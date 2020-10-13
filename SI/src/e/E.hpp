

#ifndef SI_SIGRUN_EGenerator
#define SI_SIGRUN_EGenerator

#include <sigrun/SIObject.hpp>

class E
{
public:
    static void generate();

private:
    static void parse(std::string& constants);
    static void print(std::string& constants);
};

#endif // SI_SIGRUN_EGenerator