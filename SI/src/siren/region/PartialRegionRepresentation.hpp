

#ifndef SITEST_PARTIALREGIONREPRESENTATION_HPP
#define SITEST_PARTIALREGIONREPRESENTATION_HPP

#include <QColor>
#include <QObject>
#include <glm/glm.hpp>
#include <QGraphicsPathItem>
#include <QPainterPath>
#include <memory>
#include <SI/SI.hpp>

class PartialRegionRepresentation: public QGraphicsPathItem, public SIObject
{ SIREN

public:
    PartialRegionRepresentation(const std::string& id, const std::vector<glm::vec3>& source_contour);
    ~PartialRegionRepresentation();

    void update(const std::vector<glm::vec3>& path);
    const std::string& id() const;

private:
    QColor d_color;
    std::string d_id;

};


#endif //SITEST_PARTIALREGIONREPRESENTATION_HPP
