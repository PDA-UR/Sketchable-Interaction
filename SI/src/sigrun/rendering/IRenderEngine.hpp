

#ifndef SITEST_IRENDERENGINE_HPP
#define SITEST_IRENDERENGINE_HPP

#include <QObject>

class IRenderEngine: public QObject
{Q_OBJECT
public:
    virtual void start(uint32_t width, uint32_t height, uint32_t target_fps=60) = 0;
    virtual void run() = 0;
    virtual void pause() = 0;
    virtual void stop() = 0;

    virtual void disable_anti_aliasing() = 0;
    virtual void enable_anti_aliasing(uint32_t samplng_factor) = 0;
};


#endif //SITEST_IRENDERENGINE_HPP
