//
// Created by juergen on 24/01/19.
//

#include <iostream>
#include "../include/si.h"

int main(int argc, char ** argv)
{
    si::SI *si = new si::SI(argc, argv);
    si->run();

    return 0;
}

