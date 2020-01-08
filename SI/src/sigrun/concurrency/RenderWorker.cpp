

#include "RenderWorker.hpp"

RenderWorker::RenderWorker(int width, int height, IRenderEngine* ire):
    QObject(),
    d_width(width),
    d_height(height),
    d_ire(ire)
{

}

void RenderWorker::render()
{
    d_ire->start();
}
