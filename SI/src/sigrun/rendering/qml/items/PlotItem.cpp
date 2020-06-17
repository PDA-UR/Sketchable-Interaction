

#include "PlotItem.hpp"
#include <QPainter>

PlotItem::PlotItem(QQuickItem *parent): QQuickPaintedItem(parent)
{
    setImage(QImage());
}

QImage PlotItem::image() const
{
    return m_image;
}

void PlotItem::setImage(const QImage &image)
{
    if(image == m_image)
        return;

    m_image = image;
    update();
}

void PlotItem::paint(QPainter *painter)
{
    if(m_image.isNull())
        return;

    QRectF bounding_rect = boundingRect();
    QImage scaled = m_image.scaledToHeight(bounding_rect.height());
    QPointF center = bounding_rect.center() - scaled.rect().center();

    if (center.x() < 0)
        center.setX(0);
    if (center.y() < 0)
        center.setY(0);

    painter->drawImage(center, scaled);
}