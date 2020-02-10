
#include <debug/Print.hpp>
#include "RegionRepresentation.hpp"
#include <glm/glm.hpp>
#include <QQmlEngine>
#include <QQmlContext>

RegionRepresentation::RegionRepresentation(QWidget *parent, const std::shared_ptr<Region>& region):
    d_r(region->color().r),
    d_g(region->color().g),
    d_b(region->color().b),
    d_a(region->color().a),
    d_color(QColor(d_r, d_g, d_g, d_a)),
    d_source_contour(region->contour()),
    d_qml_path(region->qml_path()),
    d_name(region->name()),
    d_view(std::make_unique<QQuickWidget>(parent)),
    d_type(region->type()),
    d_width(region->width()),
    d_height(region->height())
{
    d_fill.moveTo(d_source_contour[0].x, d_source_contour[0].y);

    for(auto& p: d_source_contour)
    {SIREN
        d_fill.lineTo(p.x, p.y);
        d_poly << QPoint(p.x, p.y);
    }

    d_poly << QPoint(d_source_contour[0].x, d_source_contour[0].y);

    d_view->engine()->rootContext()->setContextProperty("Region", this);

    if(!d_qml_path.empty())
        d_view->setSource(QUrl(QString(d_qml_path.c_str())));

    d_view->setGeometry(0, 0, 1920, 1080);
    d_view->setStyleSheet("background-color: #00000000;");
    d_view->setClearColor(Qt::transparent);
    d_view->setAttribute(Qt::WA_AlwaysStackOnTop);
    d_view->show();

    if(region->effect().has_data_changed())
        Q_EMIT dataChanged(region->data());
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
        d_fill = QPainterPath();
        d_poly = QPolygonF();

        const glm::mat3x3 &transform = region->transform();

        for (glm::vec3 &p: d_source_contour)
        {
            glm::vec3 p_ = glm::vec3(p.x, p.y, 1) * transform;

            d_poly << QPoint(p_.x / p_.z, p_.y / p_.z);
        }

        d_fill.moveTo(d_poly[0].x(), d_poly[0].y());

        for (int i = 1; i < d_poly.size(); i++)
            d_fill.lineTo(d_poly[i]);

        QMatrix4x4 matrix(transform[0].x, transform[0].y, 0, transform[0].z,
                          transform[1].x, transform[1].y, 0, transform[1].z,
                          transform[2].x, transform[2].y, 1, 0,
                          0, 0, 0, 1);

        Q_EMIT transformChanged(matrix);
    }
}

void RegionRepresentation::perform_data_update(const std::shared_ptr<Region> &region)
{
    if (region->effect().has_data_changed())
            Q_EMIT dataChanged(region->data());
}

const std::string& RegionRepresentation::name() const
{
    return d_name;
}

const QColor &RegionRepresentation::color() const
{
    return d_color;
}

const QPolygonF &RegionRepresentation::poly() const
{
    return d_poly;
}

const QPainterPath &RegionRepresentation::fill() const
{
    return d_fill;
}
