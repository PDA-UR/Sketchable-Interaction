
#include "RegionRepresentation.hpp"
#include <QQmlEngine>
#include <QLayout>
#include <QQmlComponent>
#include <algorithm>
#include <thread>
#include <QQmlProperty>
#include <QPointF>
#include <QPolygonF>
#include <QGraphicsItem>
#include <debug/Print.hpp>

RegionRepresentation::RegionRepresentation(QQmlContext* c, const std::shared_ptr<Region>& region, QGraphicsView* parent):
    d_color(QColor(region->color().r, region->color().g, region->color().b, region->color().a)),
    d_qml_path(region->qml_path()),
    d_type(region->type()),
    d_uuid(region->uuid()),
    d_name(region->name()),
    d_initial_offset(region->aabb()[0].x, region->aabb()[0].y, 1),
    d_with_border(region->effect()->is_border_present()),
    d_drawing_additions(region->effect()->drawing_additions())
{
    if(!d_qml_path.empty())
    {
        d_view = new QQuickWidget(parent);
        d_view->setSource(QUrl::fromLocalFile(d_qml_path.c_str()));

        // leave that here, otherwise after ~600 items no further views are shown
        d_view->show();

        d_view->rootContext()->setContextProperty("REGION", this);
        d_view->setGeometry(d_initial_offset.x, d_initial_offset.y, region->visualization_width(), region->visualization_height());
        d_view->setAttribute(Qt::WA_AlwaysStackOnTop);
        d_view->setAttribute(Qt::WA_NoSystemBackground);
        d_view->setClearColor(Qt::transparent);

        if(region->effect()->has_data_changed())
            QMetaObject::invokeMethod(reinterpret_cast<QObject *>(d_view->rootObject()), "updateData", QGenericReturnArgument(), Q_ARG(QVariant, region->data()));
    }

    QPolygonF poly;
    for(auto& p: region->contour())
        poly << QPointF(p.x, p.y);

    setPolygon(poly);
    setBrush(QBrush(d_color));
    setPen(QPen(d_color));
    setZValue(-1);
    setFillRule(Qt::WindingFill);
}

RegionRepresentation::~RegionRepresentation()
{
    if(!d_qml_path.empty())
    {
        d_view->close();
        delete d_view;
    }
}

const std::string& RegionRepresentation::uuid() const
{
    return d_uuid;
}

const std::string& RegionRepresentation::name() const
{
    return d_name;
}

const std::string& RegionRepresentation::qml_path() const
{
    return d_qml_path;
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
        setPolygon(QTransform()
            .translate(polygon().boundingRect().x() + (polygon().boundingRect().width() / 2), polygon().boundingRect().y() + (polygon().boundingRect().height() / 2))
            .rotate(-region->angle())
            .translate(-polygon().boundingRect().x() - (polygon().boundingRect().width() / 2), -polygon().boundingRect().y() - (polygon().boundingRect().height() / 2))
            .translate(region->last_delta_x(), region->last_delta_y())
            .map(polygon()));

        d_last_delta_x = region->x();
        d_last_delta_y = region->y();

        d_initial_offset.x += region->last_delta_x();
        d_initial_offset.y += region->last_delta_y();

        if(!d_qml_path.empty())
            d_view->move(d_initial_offset.x, d_initial_offset.y);
    }

    prepareGeometryChange();
}

void RegionRepresentation::perform_data_update(const std::shared_ptr<Region> &region)
{
    QColor color(region->color().r, region->color().g, region->color().b, region->color().a);
    bool d_visible = region->effect()->visible();

    d_border_color = QColor(region->effect()->d_border_color.r, region->effect()->d_border_color.g, region->effect()->d_border_color.b, region->effect()->d_border_color.a);
    d_border_width = region->effect()->d_border_width;
    d_with_border = region->effect()->d_with_border;

    d_border_width = d_border_width > 20 || d_border_width < 0 ? 4 : d_border_width;

    if(d_color != color)
    {
        d_color = color;
        setPen(QColor(d_color));
        setBrush(QColor(d_color));
    }

    if (region->effect()->has_data_changed())
    {
        d_drawing_additions = region->effect()->drawing_additions();

        QPolygonF poly;
        for(auto& p: region->contour())
            poly << QPointF(p.x, p.y);

        float x = this->boundingRect().x();
        float y = this->boundingRect().y();

        setPolygon(QTransform()
           .translate(poly.boundingRect().x() + (poly.boundingRect().width() / 2), poly.boundingRect().y() + (poly.boundingRect().height() / 2))
           .rotate(-region->angle())
           .translate(-poly.boundingRect().x() - (poly.boundingRect().width() / 2), -poly.boundingRect().y() - (poly.boundingRect().height() / 2))
           .translate(region->x(), region->y())
           .map(poly));

        if(!d_qml_path.empty())
        {
            if(x != this->boundingRect().x() && y != this->boundingRect().y())
            {
                d_initial_offset.x = this->boundingRect().x();
                d_initial_offset.y = this->boundingRect().y();
                d_view->move(d_initial_offset.x, d_initial_offset.y);
            }

            QMetaObject::invokeMethod(reinterpret_cast<QObject *>(d_view->rootObject()), "updateData", QGenericReturnArgument(), Q_ARG(QVariant, region->data()));
        }
    }

    if(d_was_data_received)
    {
        region->set_data(d_received_data);

        d_was_data_received = false;
    }
    prepareGeometryChange();
}

QColor& RegionRepresentation::color()
{
    return d_color;
}

void RegionRepresentation::set_data(const QVariantMap& data)
{
    d_was_data_received = true;

    d_received_data[QString("uuid")] = QVariant(d_uuid.c_str());

    for(auto& key: data.keys())
        d_received_data[key] = data[key];
}

void RegionRepresentation::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(!d_visible)
        return;
//
//    int area = polygon().boundingRect().width() * polygon().boundingRect().height();
//
//    if(area < 100)
//        return;

    QGraphicsPolygonItem::paint(painter, option, widget);


    QPen pen(QColor(0, 0, 0));
    painter->setPen(pen);
    pen.setWidth(4);

    if(d_with_border)
    {
        pen = QPen(d_border_color);
        pen.setWidth(d_border_width);

        painter->setPen(pen);
        painter->drawPolygon(this->polygon());
    }

    if(d_type == SI_TYPE_DIRECTORY || d_type == SI_TYPE_IMAGE_FILE || d_type == SI_TYPE_TEXT_FILE)
    {
        pen = QPen(QColor(72, 79, 81));
        pen.setWidth(1);
        painter->setPen(pen);

        painter->drawRect(this->polygon().boundingRect());
        painter->drawPolygon(this->polygon());
    }

    if(!d_drawing_additions.empty())
    {
        pen = QPen(QColor(0, 0, 0));
        painter->setPen(pen);
        pen.setWidth(4);

        for(std::vector<std::vector<glm::vec3>>& shape: d_drawing_additions)
        {
            for(std::vector<glm::vec3>& shape_part: shape)
            {
                QPolygonF poly;

                for(glm::vec3& p: shape_part)
                    poly << QPointF(p.x + d_last_delta_x, p.y + d_last_delta_y);

                painter->drawPolyline(poly);
            }
        }
    }
}
