
#include "PartialRegionRepresentation.hpp"

PartialRegionRepresentation::PartialRegionRepresentation(const std::string& id, const std::vector<glm::vec3>& source_contour, int stroke_width, const glm::vec4& color):
    d_id(id)
{
    if(source_contour.empty())
        return;

    QPainterPath path;

    path.moveTo(source_contour[0].x, source_contour[0].y);

    for (int i = 1; i < source_contour.size(); ++i)
        path.lineTo(source_contour[i].x, source_contour[i].y);

    QColor c(color.r, color.g, color.b, color.a);

    QPen pen(c);
    pen.setWidth(stroke_width);

    setPen(pen);
    setPath(path);
    setZValue(2);
}

PartialRegionRepresentation::~PartialRegionRepresentation()
{

}

void PartialRegionRepresentation::update(const std::vector<glm::vec3>& path)
{
    if(path.empty())
        return;

    QPainterPath qpath;

    qpath.moveTo(path[0].x, path[0].y);

    for(int i = 1; i < path.size(); ++i)
        qpath.lineTo(path[i].x, path[i].y);

    setPath(qpath);
}

const std::string &PartialRegionRepresentation::id() const
{
    return d_id;
}
