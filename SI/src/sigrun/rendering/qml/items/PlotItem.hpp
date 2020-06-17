

#ifndef SITEST_PLOTITEM_HPP
#define SITEST_PLOTITEM_HPP

#include <QImage>
#include <QQuickPaintedItem>

class PlotItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QImage image READ image WRITE setImage)

public:
    PlotItem(QQuickItem *parent = nullptr);

    QImage image() const;
    void setImage(const QImage &image);
    void paint(QPainter *painter);

private:
    QImage m_image;
};

#endif //SITEST_PLOTITEM_HPP
