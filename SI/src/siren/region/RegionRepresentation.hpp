

#ifndef SITEST_REGIONREPRESENTATION_HPP
#define SITEST_REGIONREPRESENTATION_HPP

#include <QColor>
#include <QObject>
#include <glm/glm.hpp>
#include <QQuickWidget>
#include <QQmlEngine>
#include <QVariant>
#include <QMatrix4x4>
#include <QGraphicsPolygonItem>
#include <memory>
#include <SI/SI.hpp>

class RegionRepresentation: public QGraphicsPolygonItem, public SIObject
{ SIREN

public:
    RegionRepresentation(QQmlEngine* e, const std::shared_ptr<Region>& region);
    ~RegionRepresentation();

    void update(const std::shared_ptr<Region>& region);
    const std::string& uuid() const;
    const std::string& name() const;

    QQuickWidget& view();
    QColor& color();

private:
    void perform_transform_update(const std::shared_ptr<Region>& region);
    void perform_data_update(const std::shared_ptr<Region>& region);

    QColor d_color;
    std::string d_qml_path;
    std::string d_uuid;
    std::string d_name;
    QQuickWidget d_view;
    uint32_t d_type;

    glm::vec3 d_initial_offset;
};

#endif //SITEST_REGIONREPRESENTATION_HPP
