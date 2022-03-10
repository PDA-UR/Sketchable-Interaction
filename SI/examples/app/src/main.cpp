#include <SI/SI.hpp>
#include <sirose/SIROS2Environment.hpp>

int main(int argc, char** argv)
{
    Log::__DEBUG__ = true;
    Log::SHOW = LOG_SHOW_ALL;
    Log::WHERE = LOG_CONSOLE;

    RenderEngineQT5 re;
    SIROS2Environment ros;

    SIGRun().exec(argc, argv, &re, &ros);
//    SIGRun().exec(argc, argv, &re, nullptr);

    return 0;
}