

#ifndef SITEST_REGIONREPRESENTATION_HPP
#define SITEST_REGIONREPRESENTATION_HPP

#include <QColor>
#include <QObject>
#include <glm/glm.hpp>
#include <QQuickWidget>
#include <QVariant>
#include <QMatrix4x4>
#include <memory>
#include <SI/SI.hpp>

class RegionRepresentation: public QObject, public SIObject
{ Q_OBJECT

public:
    RegionRepresentation(QWidget* parent, const std::shared_ptr<Region>& region);

    void update(const std::shared_ptr<Region>& region);

    Q_SIGNAL void transformChanged(const QMatrix4x4& tform);
    Q_SIGNAL void dataChanged(const QMap<QString, QVariant>& data);

    const std::string& name() const;
    const QColor& color() const;
    const QPainterPath& fill() const;

private:
    void perform_transform_update(const std::shared_ptr<Region>& region);
    void perform_data_update(const std::shared_ptr<Region>& region);

    int d_r;
    int d_g;
    int d_b;
    int d_a;
    int d_type;
    int d_width;
    int d_height;
    QColor d_color;
    std::string d_qml_path;
    std::string d_name;
    QPainterPath d_fill;
    std::vector<glm::vec3> d_source_contour;
    std::unique_ptr<QQuickWidget> d_view;
};

#endif //SITEST_REGIONREPRESENTATION_HPP
