
#include "PartialRegionRepresentation.hpp"

PartialRegionRepresentation::PartialRegionRepresentation(const std::string& id, const std::vector<glm::vec3>& source_contour):
    d_id(id)
{
    QPainterPath path;

    path.moveTo(source_contour[0].x, source_contour[0].y);

    std::for_each(source_contour.begin() + 1, source_contour.end(), [&](const auto& p)
    {
        path.lineTo(p.x, p.y);
    });

    QPen pen(QColor(72, 79, 81)); // 177, 180, 181
    pen.setWidth(4);

    setPen(pen);
    setPath(path);
    setZValue(2);
}

PartialRegionRepresentation::~PartialRegionRepresentation()
{

}

void PartialRegionRepresentation::update(const std::vector<glm::vec3>& path)
{
    QPainterPath qpath;

    qpath.moveTo(path[0].x, path[0].y);

    std::for_each(path.begin() + 1, path.end(), [&](const auto& p)
    {
        qpath.lineTo(p.x, p.y);
    });

    setPath(qpath);
}

const std::string &PartialRegionRepresentation::id() const
{
    return d_id;
}
