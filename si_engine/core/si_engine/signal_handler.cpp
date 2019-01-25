//
// Created by juergen on 22/01/19.
//

#include "signal_handler.h"

namespace si
{
    void signal_handler::handle(int signum)
    {
        switch (signum)
        {
            case SIGINT:
                exit(0);

            default:
                debug::print("Interrupt Signal (", signum, ") received.");
                exit(signum);
        }
    }
}

