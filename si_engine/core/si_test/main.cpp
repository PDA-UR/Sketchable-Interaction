//
// Created by juergen on 24/01/19.
//

#include <iostream>
#include "../si.h"

int main(int argc, char ** argv)
{
    si::SI *si = new si::SI(argc, argv);
    si->run();

    /*
    void *si = si::si_create_instance(argc, argv);

    si::si_run(si);
    si::si_delete_instance(si);
    */
    return 0;
}

