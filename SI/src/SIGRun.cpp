
#include <memory>
#include <iostream>

#include "SIGRun.hpp"
#include "debug/Print.hpp"

std::unique_ptr<Core> SIGRun::up_core(new Core());

SIGRun::SIGRun()
{

}

SIGRun::~SIGRun()
{

}

/**
    \brief entry point of SIGRun
    @details
        Entry point of SIGRun initializing all further systems
    @param argc cli argc
    @param argv cli argv
*/
int SIGRun::exec(int argc, char **argv)
{
    up_core->start();

    return 0;
}

/**
    \brief exit SIGRun
    @details
        static exit function of SIGRun terminating all other systems
*/
int SIGRun::quit()
{
    up_core->stop();

    return 0;
}
