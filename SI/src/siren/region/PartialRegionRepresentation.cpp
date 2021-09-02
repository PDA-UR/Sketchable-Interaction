
#include "PartialRegionRepresentation.hpp"

PartialRegionRepresentation::PartialRegionRepresentation(const std::string& id, const std::vector<glm::vec3>& source_contour, int color):
    d_id(id)
{
    QPainterPath path;

    path.moveTo(source_contour[0].x, source_contour[0].y);

    std::for_each(source_contour.begin() + 1, source_contour.end(), [&](const auto& p)
    {
        path.lineTo(p.x, p.y);
    });

    QColor c(72, 79, 81); // 177, 180, 181

    if(color == SI_BLACK_DRAWING_COLOR)
        c = QColor(72, 79, 81); // 177, 180, 181

    if(color == SI_WHITE_DRAWING_COLOR)
        c = QColor(255 - 72, 255 - 79, 255 - 81);

    QPen pen(c);

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
