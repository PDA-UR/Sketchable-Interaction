#include <SI/SI.hpp>

int main(int argc, char** argv)
{
    Log::__DEBUG__ = true;
    Log::SHOW = LOG_SHOW_ALL;
    Log::WHERE = LOG_CONSOLE;

    Log::quench("LINKINGMANAGER");
    
    SIGRun s;
    RenderEngineQT5 re;

    s.exec(argc, argv, &re);

    return 0;
}