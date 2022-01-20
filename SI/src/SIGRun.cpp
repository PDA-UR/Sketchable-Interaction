
#include <memory>
#include <iostream>
#include <signal.h>
#include <sigrun/log/CrashDump.hpp>

#include "SIGRun.hpp"
#include "debug/Print.hpp"

/**
    \brief initialization of the Core object as a static unique_ptr
*/
std::unique_ptr<Core> SIGRun::up_core(new Core());

/**
    \brief constructor
    \details
        Constructor of SIGRun class.
        Used for instantiating objects.
*/
SIGRun::SIGRun()
{

}

/**
    \brief destructor
    \details
        Destructor of SIGRun class.
        Used for destroying objects.
*/
SIGRun::~SIGRun()
{

}

/**
    \brief entry point of SIGRun
    @details
        Entry point of SIGRun initializing all further systems.
    @param argc cli argc
    @param argv cli argv
*/
int SIGRun::exec(int argc, char **argv, IRenderEngine* ire, IPhysicalEnvironment* ros)
{
    up_core->start(argv, argc, ire, ros);

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
