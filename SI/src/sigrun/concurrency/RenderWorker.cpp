

#include "RenderWorker.hpp"

RenderWorker::RenderWorker(int width, int height, IRenderEngine* ire):
    QObject(),
    d_width(width),
    d_height(height)
{

    d_ire = std::make_unique<IRenderEngine*>(ire);
}

void RenderWorker::render()
{
    (*d_ire)->start(d_width, d_height);
}
