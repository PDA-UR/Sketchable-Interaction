

#ifndef SITEST_IRENDERENGINE_HPP
#define SITEST_IRENDERENGINE_HPP


#include <sigrun/concurrency/StoppableTask.hpp>

#include <QObject>

class IRenderEngine: public QObject
{Q_OBJECT
public:
    virtual void start(int width, int height) = 0;
    virtual void run() = 0;
    virtual void pause() = 0;
    virtual void stop() = 0;
};


#endif //SITEST_IRENDERENGINE_HPP