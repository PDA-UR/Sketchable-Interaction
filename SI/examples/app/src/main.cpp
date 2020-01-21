#include <SI/SI.hpp>

int main(int argc, char** argv)
{
    Log::SHOW = LOG_SHOW_ALL;

    SIGRun s;
    RenderEngineQT5 re;

    s.exec(argc, argv, &re);

    return 0;
}