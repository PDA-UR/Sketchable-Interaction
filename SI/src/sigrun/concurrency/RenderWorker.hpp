

#ifndef SITEST_RENDERWORKER_HPP
#define SITEST_RENDERWORKER_HPP

#include <QObject>
#include <sigrun/rendering/IRenderEngine.hpp>

class RenderWorker: public QObject
{Q_OBJECT
public:
    RenderWorker(int width, int height, IRenderEngine* ire);

    Q_SLOT void render();

    Q_SIGNAL void finished();

private:
    int d_width;
    int d_height;
    std::unique_ptr<IRenderEngine*> d_ire;

protected:

};


#endif //SITEST_RENDERWORKER_HPP
