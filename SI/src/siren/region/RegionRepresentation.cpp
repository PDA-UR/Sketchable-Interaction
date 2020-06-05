
#include "RegionRepresentation.hpp"
#include <glm/glm.hpp>
#include <QQmlEngine>
#include <QLayout>
#include <execution>
#include <algorithm>
#include <numeric>
#include <thread>
#include <QQmlContext>
#include <QQmlProperty>

RegionRepresentation::RegionRepresentation(QWidget *parent, QQmlEngine* engine, const std::shared_ptr<Region>& region):
    d_color(QColor(region->color().r, region->color().g, region->color().b, region->color().a)),
    d_qml_path(region->qml_path()),
    d_view(new QQuickWidget(engine, this)),
    d_type(region->type()),
    d_uuid(region->uuid()),
    d_name(region->name())
{
    if(!d_qml_path.empty())
        d_view->setSource(QUrl::fromLocalFile(QString(d_qml_path.c_str())));

    d_view->setGeometry(0, 0, region->aabb()[3].x - region->aabb()[0].x, region->aabb()[1].y - region->aabb()[0].y);
    d_view->setParent(this);
    d_view->setAttribute(Qt::WA_AlwaysStackOnTop);
    d_view->setAttribute(Qt::WA_NoSystemBackground);
    d_view->setClearColor(Qt::transparent);

    setParent(parent);
    setGeometry(region->aabb()[0].x, region->aabb()[0].y, region->aabb()[3].x - region->aabb()[0].x, region->aabb()[1].y - region->aabb()[0].y);

    if(region->effect()->has_data_changed())
        QMetaObject::invokeMethod(reinterpret_cast<QObject *>(d_view->rootObject()), "updateData", QGenericReturnArgument(), Q_ARG(QVariant, region->data()));

    d_fill.moveTo(region->contour()[0].x - region->aabb()[0].x, region->contour()[0].y - region->aabb()[0].y);

    std::for_each(region->contour().begin() + 1, region->contour().end(), [&](auto& point)
    {
        d_fill.lineTo(point.x - region->aabb()[0].x, point.y - region->aabb()[0].y);
    });

    show();
}

RegionRepresentation::~RegionRepresentation()
{
    d_view->close();
    delete d_view;
    d_view = nullptr;
}

const uint32_t RegionRepresentation::type() const
{
    return d_type;
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
        if(region->type() == SI_TYPE_EXTERNAL_APPLICATION_CONTAINER)
            this->move(region->transform()[0].z, region->transform()[1].z);
        else
            this->move(region->transform()[0].z + region->aabb()[0].x, region->transform()[1].z + region->aabb()[0].y);
    }
}

void RegionRepresentation::perform_data_update(const std::shared_ptr<Region> &region)
{
    if (region->effect()->has_data_changed())
    {
        resize(region->aabb()[3].x - region->aabb()[0].x, region->aabb()[1].y - region->aabb()[0].y);

        d_color = QColor(region->color().r, region->color().g, region->color().b, region->color().a);

        d_fill = QPainterPath();

        d_fill.moveTo(region->contour()[0].x - region->aabb()[0].x, region->contour()[0].y - region->aabb()[0].y);

        std::for_each(region->contour().begin() + 1, region->contour().end(), [&](auto& point)
        {
            d_fill.lineTo(point.x - region->aabb()[0].x, point.y - region->aabb()[0].y);
        });

        QMetaObject::invokeMethod(reinterpret_cast<QObject *>(d_view->rootObject()), "updateData", QGenericReturnArgument(), Q_ARG(QVariant, region->data()));
    }
}

void RegionRepresentation::paintEvent(QPaintEvent *event)
{
    up_qp.begin(this);
    up_qp.fillPath(d_fill,d_color);

    if(d_type == SI_TYPE_CANVAS)
    {
        const auto& partial_regions = Context::SIContext()->region_manager()->partial_regions();
        up_qp.setBrush(QColor(255, 255, 255));

        for(const auto& [k, v]: partial_regions)
        {
            QPolygonF partial_poly;

            for(const auto& p: v)
                partial_poly << QPointF(p.x, p.y);

            up_qp.drawPolyline(partial_poly);
        }
    }

    up_qp.end();
}
