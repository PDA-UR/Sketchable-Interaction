

#ifndef SITEST_REGIONREPRESENTATION_HPP
#define SITEST_REGIONREPRESENTATION_HPP

#include <QColor>
#include <QObject>
#include <glm/glm.hpp>
#include <QQuickWidget>
#include <QQmlEngine>
#include <QQmlContext>
#include <QVariant>
#include <QMatrix4x4>
#include <QGraphicsPolygonItem>
#include <memory>
#include <SI/SI.hpp>

class RegionRepresentation: public QObject, public QGraphicsPolygonItem, public SIObject
{ Q_OBJECT SIREN

public:
    RegionRepresentation(QQmlContext* c, const std::shared_ptr<Region>& region, QGraphicsView* parent);
    ~RegionRepresentation();

    void update(const std::shared_ptr<Region>& region);
    const std::string& uuid() const;
    const std::string& name() const;
    const std::string& qml_path() const;

    QColor& color();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    Q_SLOT void set_data(const QVariantMap& data);

private:
    void perform_transform_update(const std::shared_ptr<Region>& region);
    void perform_data_update(const std::shared_ptr<Region>& region);

    QColor d_color;
    QColor d_border_color = QColor(72, 79, 81);
    std::string d_qml_path;
    std::string d_uuid;
    std::string d_name;
    QQuickWidget* d_view;
    uint32_t d_type;
    int d_border_width = 4;

    float d_last_angle = 0.0f;

    int d_last_delta_x = 0;
    int d_last_delta_y = 0;

    bool d_was_data_received = false;
    bool d_with_border = true;
    bool d_visible = true;

    QMap<QString, QVariant> d_received_data;

    glm::vec3 d_initial_offset;

    std::vector<std::vector<std::vector<glm::vec3>>> d_drawing_additions;
};

#endif //SITEST_REGIONREPRESENTATION_HPP
