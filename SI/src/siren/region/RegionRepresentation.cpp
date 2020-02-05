
#include "RegionRepresentation.hpp"

RegionRepresentation::RegionRepresentation(const std::vector<glm::vec3> &c, const glm::mat3x3 &t, const glm::vec4 &col,
                                           const std::string &_name, const std::string &tex_path, QWidget *parent):
    r(col.r),
    g(col.g),
    b(col.b),
    a(col.a),
    color(QColor(col.r, col.g, col.b, col.a)),
    source_contour(c),
    texture_path(tex_path),
    name(_name),
    view(std::make_unique<QQuickWidget>(parent))
{
    fill.moveTo(c[0].x, c[0].y);

    for(auto& p: c)
    {
        fill.lineTo(p.x, p.y);
        poly << QPoint(p.x, p.y);
    }

    poly << QPoint(c[0].x, c[0].y);

    view->setSource(QUrl("plugins/standard_environment_library/cursor/qml/test.qml"));
    view->setGeometry(0, 0, 1920, 1080);
    view->setStyleSheet("background-color: #00000000;");
    view->setClearColor(Qt::transparent);
    view->setAttribute(Qt::WA_AlwaysStackOnTop);
    view->show();
}

void RegionRepresentation::update(const glm::mat3x3 &transform)
{
    fill = QPainterPath();
    poly = QPolygonF();

    for(glm::vec3& p: source_contour)
    {
        glm::vec3 p_ = glm::vec3(p.x, p.y, 1) * transform;
        p_.x /= p_.z;
        p_.y /= p_.z;

        poly << QPoint(p_.x, p_.y);
    }

    fill.moveTo(poly[0].x(), poly[0].y());

    for(int i = 1; i < poly.size(); i++)
        fill.lineTo(poly[i]);
}
