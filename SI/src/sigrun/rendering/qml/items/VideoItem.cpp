

#include "VideoItem.hpp"

VideoItem::VideoItem(QObject* parent) : QObject(parent)
{
}

QAbstractVideoSurface *VideoItem::videoSurface() const
{
    return d_surface;
}

void VideoItem::setVideoSurface(QAbstractVideoSurface *surface)
{
    if (d_surface == surface)
        return;
    if(d_surface && d_surface != surface && d_surface->isActive())
        d_surface->stop();
    d_surface = surface;

    if (d_surface && d_format.isValid()) {
        d_format = d_surface->nearestFormat(d_format);
        d_surface->start(d_format);
    }
}

void VideoItem::set_format(int width, int height, QVideoFrame::PixelFormat frame_format)
{
    QSize size(width, height);
    QVideoSurfaceFormat format(size, frame_format);
    d_format = format;
    if (d_surface) {
        if (d_surface->isActive())
            d_surface->stop();

        d_format = d_surface->nearestFormat(d_format);
        d_surface->start(d_format);
    }
}

void VideoItem::onVideoFrameReady(QImage image)
{
    if(!d_surface->isActive() and !d_format.isValid())
        set_format(image.width(), image.height(), QVideoFrame::Format_RGB32);

    d_surface->present(QVideoFrame(image));
}
