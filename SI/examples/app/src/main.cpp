#include <SI/SI.hpp>

int main(int argc, char** argv)
{
    Log::__DEBUG__ = true;
    Log::SHOW = LOG_SHOW_ALL;
    Log::WHERE = LOG_CONSOLE;

    RenderEngineQT5 re;

    SIGRun().exec(argc, argv, &re);

    return 0;
}