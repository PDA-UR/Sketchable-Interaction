
#include <memory>
#include <iostream>

#include "SIGRun.hpp"

SIGRun::SIGRun() : up_core(std::unique_ptr<Core>(new Core()))
{

}

SIGRun::~SIGRun()
{
}

int SIGRun::exec(int argc, char **argv)
{
    up_core->start();

    return 0;
}
