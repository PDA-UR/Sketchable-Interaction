
#ifndef SI_SIGRUN_SIGRUN_H
#define SI_SIGRUN_SIGRUN_H

#include <memory>
#include "sigrun/Core.hpp"

class SIGRun
{
public:
    SIGRun();
    ~SIGRun();

    int exec(int argc, char** argv);

private:
    std::unique_ptr<Core> up_core;
};

#endif //SI_SIGRUN_SIGRUN_H
