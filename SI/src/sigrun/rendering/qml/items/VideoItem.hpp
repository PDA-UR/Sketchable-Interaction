

#ifndef SI_SIGRUN_VideoItem
#define SI_SIGRUN_VideoItem

#include <QPointer>
#include <QVideoFrame>
#include <QVideoSurfaceFormat>
#include <QAbstractVideoSurface>
#include <QImage>

class VideoItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QAbstractVideoSurface* videoSurface READ videoSurface WRITE setVideoSurface)

public:
    VideoItem(QObject* parent= nullptr);
    QAbstractVideoSurface* videoSurface() const;

    Q_SLOT void setVideoSurface(QAbstractVideoSurface* surface);
    Q_SLOT void onVideoFrameReady(QImage image);

private:
    void set_format(int width, int height, QVideoFrame::PixelFormat format);

    QPointer<QAbstractVideoSurface> d_surface;
    QVideoSurfaceFormat d_format;
};

#endif // SI_SIGRUN_VideoItem