

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

class RegionRepresentation: public QWidget, public SIObject
{ Q_OBJECT SIREN

public:
    RegionRepresentation(QWidget* parent, const std::shared_ptr<Region>& region);
    ~RegionRepresentation();

    void update(const std::shared_ptr<Region>& region);
    const uint32_t type() const;
    const std::string& uuid() const;
    const std::string& name() const;

    Q_SIGNAL void dataChanged(const QMap<QString, QVariant>& data);

protected:
    QPainter up_qp;
    void paintEvent(QPaintEvent *event) override;

private:
    void perform_transform_update(const std::shared_ptr<Region>& region);
    void perform_data_update(const std::shared_ptr<Region>& region);

    QColor d_color;
    std::string d_qml_path;
    std::string d_uuid;
    std::string d_name;
    QPainterPath d_fill;
    std::unique_ptr<QQuickWidget> d_view;
    uint32_t d_type;

    QMetaObject::Connection d_region_connection;
};

#endif //SITEST_REGIONREPRESENTATION_HPP
