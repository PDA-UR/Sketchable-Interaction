

#ifndef SITEST_REGIONREPRESENTATION_HPP
#define SITEST_REGIONREPRESENTATION_HPP

#include <QColor>
#include <glm/glm.hpp>

struct RegionRepresentation
{
    RegionRepresentation(const std::vector<glm::vec3>& c, const glm::mat3x3& t, const QColor& col, const std::string& tex_path):
        r(col.red()),
        g(col.green()),
        b(col.blue()),
        a(col.alpha()),
        color(col),
        texture_path(tex_path)
    {
        fill.moveTo(c[0].x, c[0].y);

        for(auto& p: c)
        {
            fill.lineTo(p.x, p.y);
            poly << QPoint(p.x, p.y);
        }

        poly << QPoint(c[0].x, c[0].y);
    }

    RegionRepresentation(const std::vector<glm::vec3>& c, const glm::mat3x3& t, int r_, int g_, int b_, int a_, const std::string& tex_path):
        r(r_),
        g(g_),
        b(b_),
        a(a_),
        color(QColor(r, g, b, a)),
        texture_path(tex_path)
    {
        fill.moveTo(c[0].x, c[0].y);

        for(auto& p: c)
        {
            fill.lineTo(p.x, p.y);
            poly << QPoint(p.x, p.y);
        }

        poly << QPoint(c[0].x, c[0].y);
    }

    void update(const glm::mat3x3& transform)
    {
        fill = QPainterPath();

        for(QPointF& p: poly)
        {
            glm::vec3 p_ = glm::vec3(p.x(), p.y(), 1) * transform;
            p.setX(p_.x);
            p.setY(p_.y);
        }

        fill.moveTo(poly[0].x(), poly[0].y());

        for(int i = 1; i < poly.size(); i++)
            fill.lineTo(poly[i]);
    }

    int r;
    int g;
    int b;
    int a;
    QColor color;
    std::string texture_path;
    QPolygonF poly;
    QPainterPath fill;
};

#endif //SITEST_REGIONREPRESENTATION_HPP
