

#ifndef SITEST_REGIONREPRESENTATION_HPP
#define SITEST_REGIONREPRESENTATION_HPP

#include <QColor>
#include <glm/glm.hpp>
#include <QQuickWidget>
#include <memory>

struct RegionRepresentation
{
    RegionRepresentation(const std::vector<glm::vec3>& c, const glm::mat3x3& t, const glm::vec4& col, const std::string& _name, const std::string& tex_path, QWidget* parent);

    void update(const glm::mat3x3& transform);

    int r;
    int g;
    int b;
    int a;
    QColor color;
    std::string texture_path;
    std::string name;
    QPolygonF poly;
    QPainterPath fill;
    std::vector<glm::vec3> source_contour;
    std::unique_ptr<QQuickWidget> view;
};

#endif //SITEST_REGIONREPRESENTATION_HPP
