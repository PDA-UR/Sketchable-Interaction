
#ifndef SI_SIGRUN_SIGRUN_H
#define SI_SIGRUN_SIGRUN_H

#include <memory>
#include "sigrun/Core.hpp"
#include "sigrun/SIObject.hpp"


/**
\class SIGRun
\brief SIGRun class serving as entry point of an SI environment
\details
    This class serves as the entry point of an SI environment.
    It is directly exposed in SI.hpp.
    An instance of this class is used to launch an SI environment.
@see up_core
*/
class SIGRun: public SIObject
{ SIGRUN
public:
    SIGRun();
    ~SIGRun();

    int exec(int argc, char **argv, IRenderEngine* ire, IROS2Environment* ros);

    static int quit();

private:
    /**
    \brief static unique ptr to a Core object
    \details
        The SIGRun Core instance from which all further subsystems of the environment are launched.
        Due to its size and functionality, it is stored as a static unique_ptr.
     */
    static std::unique_ptr<Core> up_core;
};

#endif //SI_SIGRUN_SIGRUN_H
