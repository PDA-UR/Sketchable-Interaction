
#include "RegionRepresentation.hpp"
#include <QQmlEngine>
#include <QLayout>
#include <algorithm>
#include <thread>
#include <QQmlProperty>
#include <QPointF>
#include <QPolygonF>
#include <QGraphicsItem>

RegionRepresentation::RegionRepresentation(QQmlEngine* e, const std::shared_ptr<Region>& region):
    d_color(QColor(region->color().r, region->color().g, region->color().b, region->color().a)),
    d_qml_path(region->qml_path()),
    d_view(QQuickWidget(e, nullptr)),
    d_type(region->type()),
    d_uuid(region->uuid()),
    d_name(region->name()),
    d_initial_offset(region->aabb()[0].x, region->aabb()[0].y, 1)
{
    if(!d_qml_path.empty())
        d_view.setSource(QUrl::fromLocalFile(QString(d_qml_path.c_str())));

    d_view.setGeometry(0, 0, region->aabb()[3].x - region->aabb()[0].x, region->aabb()[1].y - region->aabb()[0].y);
    d_view.setAttribute(Qt::WA_AlwaysStackOnTop);
    d_view.setAttribute(Qt::WA_NoSystemBackground);
    d_view.setClearColor(Qt::transparent);
    d_view.move(d_initial_offset.x, d_initial_offset.y);

    if(region->effect()->has_data_changed())
        QMetaObject::invokeMethod(reinterpret_cast<QObject *>(d_view.rootObject()), "updateData", QGenericReturnArgument(), Q_ARG(QVariant, region->data()));

    QPolygonF poly;
    for(auto& p: region->contour())
        poly << QPointF(p.x, p.y);

    setPolygon(poly);
    setBrush(QBrush(d_color));
    setPen(QPen(d_color));
    setZValue(-1);
}

RegionRepresentation::~RegionRepresentation()
{
	d_view.close();
}

const std::string& RegionRepresentation::uuid() const
{
    return d_uuid;
}

const std::string& RegionRepresentation::name() const
{
    return d_name;
}

void RegionRepresentation::update(const std::shared_ptr<Region>& region)
{
    perform_transform_update(region);
    perform_data_update(region);
}

void RegionRepresentation::perform_transform_update(const std::shared_ptr<Region> &region)
{
    if(region->is_transformed())
    {
        int x = d_initial_offset.x + region->transform()[0].z;
        int y = d_initial_offset.y + region->transform()[1].z;

        setPos(region->transform()[0].z, region->transform()[1].z);
        d_view.move(x, y);
    }
}

void RegionRepresentation::perform_data_update(const std::shared_ptr<Region> &region)
{
    if (region->effect()->has_data_changed())
    {
        d_color = QColor(region->color().r, region->color().g, region->color().b, region->color().a);

        QPolygonF poly;
        for(auto& p: region->contour())
            poly << QPointF(p.x, p.y);

        setPolygon(poly);
        setPen(QColor(d_color));
        setBrush(QColor(d_color));

        QMetaObject::invokeMethod(reinterpret_cast<QObject *>(d_view.rootObject()), "updateData", QGenericReturnArgument(), Q_ARG(QVariant, region->data()));
    }
}

QQuickWidget& RegionRepresentation::view()
{
    return d_view;
}

QColor& RegionRepresentation::color()
{
    return d_color;
}
