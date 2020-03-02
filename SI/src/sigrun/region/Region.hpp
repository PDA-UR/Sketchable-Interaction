

#ifndef SITEST_REGION_HPP
#define SITEST_REGION_HPP

#include <boost/python.hpp>

#include <memory>
#include <glm/glm.hpp>

#include "RegionMask.hpp"
#include "RegionTransform.hpp"
#include "sigrun/plugin/PythonInvoker.hpp"
#include <sigrun/SIObject.hpp>
#include <sigrun/util/RingBuffer.hpp>
#include <QObject>
#include <pysi/PySIEffect.hpp>

namespace bp = boost::python;

Q_DECLARE_METATYPE (bp::object)
Q_DECLARE_METATYPE (bp::tuple)


class Region: public QObject, public SIObject
{Q_OBJECT
public:
    Region(const std::vector<glm::vec3>& contour, const bp::object& effect, int width=0, int height=0, const bp::dict& kwargs=bp::dict());
    ~Region();

    bool is_transformed() const;
    void set_is_transformed(bool b);
    const std::string& uuid() const;

    PySIEffect& effect();
    bp::object& raw_effect();

    const std::unique_ptr<RegionMask>& mask() const;

    const std::vector<glm::vec3>& aabb();
    const std::vector<glm::vec3>& contour();

    void set_aabb();

    const std::string& qml_path() const;

    void move(int x, int y);

    const glm::mat3x3& transform() const;

    int on_enter(PySIEffect& other);
    int on_continuous(PySIEffect& other);
    int on_leave(PySIEffect& other);

    Q_SIGNAL void LINK_SIGNAL(const std::string& uuid, const std::string& source_cap, const bp::tuple& args);
    Q_SLOT void LINK_SLOT(const std::string& uuid, const std::string& source_cap, const bp::tuple& args);
    Q_SLOT void REGION_DATA_CHANGED_SLOT(const QMap<QString, QVariant>& data);

    void register_link_event(const std::string& uuid, const std::string& attribute);
    void register_link_event(const std::tuple<std::string, std::string>& link_event);

    bool is_link_event_registered(const std::string& uuid, const std::string& attribute);
    bool is_link_event_registered(const std::tuple<std::string, std::string>& link_event);

    const std::string& name() const;
    const glm::vec4 color() const;
    const int type() const;
    const int width() const;
    const int height() const;

    int handle_collision_event(const std::string& function_name, PySIEffect& colliding_effect);

    void update();
    const QMap<QString, QVariant>& data() const;

    const int last_delta_x() const;
    const int last_delta_y() const;

private:
    void process_canvas_specifics();
    void process_linking_relationships();
    void process_contour_change();

    std::shared_ptr<PySIEffect> d_py_effect;
    std::shared_ptr<bp::object> d_effect;

    std::vector<glm::vec3> d_contour;
    std::vector<glm::vec3> d_aabb;

    RingBuffer<std::tuple<std::string, std::string>> d_link_events;

    std::unique_ptr<RegionMask> uprm;
    std::unique_ptr<RegionTransform> uprt;
    bool d_is_transformed;

    int d_last_x;
    int d_last_y;
    int d_last_delta_x;
    int d_last_delta_y;
};


#endif //SITEST_REGION_HPP
