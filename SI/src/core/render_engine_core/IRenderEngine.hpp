

#ifndef SI_IRENDERENGINE_HPP
#define SI_IRENDERENGINE_HPP


#include <core/concurrency/StoppableTask.hpp>
namespace SI
{
    class IRenderEngine : public StoppableTask
    {
    public:
        virtual void start(int width, int height) = 0;

        virtual void run() = 0;

        virtual void pause() = 0;
    };
}

#endif //SI_IRENDERENGINE_HPP
