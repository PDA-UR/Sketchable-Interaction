#include <SI/SI.hpp>

int main(int argc, char** argv)
{
    RenderEngineSDL2 re;
    SIGRun s;
    s.exec(argc, argv, &re);

    return 0;
}