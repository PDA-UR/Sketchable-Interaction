

#ifndef SITEST_REGION_HPP
#define SITEST_REGION_HPP

#include <boost/python.hpp>

#include <memory>
#include <glm/glm.hpp>

#include "RegionMask.hpp"
#include "RegionTransform.hpp"
#include "sigrun/plugin/PythonInvoker.hpp"
#include <sigrun/SIObject.hpp>
#include <QObject>

namespace bp = boost::python;

class Region: public QObject, public SIObject
{Q_OBJECT
public:
    Region(const std::vector<glm::vec3>& contour, std::shared_ptr<bp::object> effect);
    ~Region();

    const bool is_transformed() const;
    void set_is_transformed(bool b);
    const std::string uuid() const;

    bp::object& effect();

    const std::unique_ptr<RegionMask>& mask() const;

    const std::vector<glm::vec3>& aabb();
    const std::vector<glm::vec3>& contour();

    void set_aabb();

    const std::string& texture_path() const;

    void move(int x, int y);

    const glm::mat3x3& transform() const;

    int on_enter(bp::object& other);
    int on_continuous(bp::object& other);
    int on_leave(bp::object& other);

    Q_SIGNAL void __position__(int x, int y, const std::string& event_uuid);

    int __x__ = 0;
    int __y__ = 0;
private:
    Q_SLOT void __set_position__(int x, int y, const std::string& event_uuid);

    std::vector<glm::vec3> d_contour;
    std::vector<glm::vec3> d_aabb;

    std::vector<std::string> d_link_events;

    std::unique_ptr<RegionMask> uprm;
    std::unique_ptr<RegionTransform> uprt;
    std::unique_ptr<PythonInvoker> uppi;
    std::shared_ptr<bp::object> d_effect;
    std::string d_uuid;
    bool d_is_transformed;

    std::string d_texture_path_default;
    std::string d_name;
};


#endif //SITEST_REGION_HPP
