

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
    Region(const std::vector<glm::vec3>& contour, const bp::object& effect, int width=0, int height=0);
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

    void register_link_event(const std::string& uuid, const std::string& attribute);
    void register_link_event(const std::tuple<std::string, std::string>& link_event);

    bool is_link_event_registered(const std::string& uuid, const std::string& attribute);
    bool is_link_event_registered(const std::tuple<std::string, std::string>& link_event);

    void set_name(const std::string& name);
    const std::string& name() const;

    void set_color(const glm::vec4& color);
    const glm::vec4 color() const;

    const int type() const;
    int set_type(int type);

    const int width() const;
    const int height() const;

    int handle_collision_event(const std::string& function_name, PySIEffect& colliding_effect);

    void update();
    const QMap<QString, QVariant>& data() const;

private:
    std::shared_ptr<PySIEffect> d_py_effect;
    std::shared_ptr<bp::object> d_effect;

    std::vector<glm::vec3> d_contour;
    std::vector<glm::vec3> d_aabb;

    RingBuffer<std::tuple<std::string, std::string>> d_link_events;

    std::unique_ptr<RegionMask> uprm;
    std::unique_ptr<RegionTransform> uprt;
    std::string d_uuid;
    bool d_is_transformed;

    std::string d_qml_path_default;
    std::string d_name;
    int d_type;
    int d_width;
    int d_height;

    glm::vec4 d_color;

    std::vector<std::string> d_attributes_emit;
    std::unordered_map<std::string, std::vector<std::string>> d_attributes_recv;
};


#endif //SITEST_REGION_HPP
