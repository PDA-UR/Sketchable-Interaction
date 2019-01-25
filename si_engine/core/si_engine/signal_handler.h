//
// Created by juergen on 22/01/19.
//

#ifndef CORE_SIGNAL_HANDLER_H
#define CORE_SIGNAL_HANDLER_H

#include <csignal>
#include "../si_debug/debug.h"

namespace si
{
    class signal_handler
    {
    public:
        enum SIGNAL
        {
            NONE,
            ABRT,
            FPE,
            ILL,
            INT,
            SEGV,
            TERM,
        };

        static void handle(int signum);
    };
}



#endif //CORE_SIGNAL_HANDLER_H
