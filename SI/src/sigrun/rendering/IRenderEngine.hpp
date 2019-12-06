

#ifndef SITEST_IRENDERENGINE_HPP
#define SITEST_IRENDERENGINE_HPP


#include <sigrun/concurrency/StoppableTask.hpp>

class IRenderEngine: public StoppableTask
{
public:
    virtual void start(int width, int height, int argc, char** argv) = 0;
    virtual void run() = 0;
    virtual  void pause() = 0;
};


#endif //SITEST_IRENDERENGINE_HPP
