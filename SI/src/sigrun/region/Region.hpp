

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


class Region: public QObject, public SIObject
{ Q_OBJECT SIGRUN
public:
    Region(const std::vector<glm::vec3>& contour, const bp::object& effect, uint32_t width=0, uint32_t height=0, const bp::dict& kwargs=bp::dict());
    ~Region();

    bool is_transformed() const;
    void set_is_transformed(bool b);
    const std::string& uuid() const;

    void set_effect(const bp::object& effect, const bp::dict& kwargs);
    void set_effect(const std::vector<glm::vec3>& contour, const bp::object& effect, const std::string& uuid, const bp::dict& kwargs);

    void set_data(const QMap<QString, QVariant>& data);

    PySIEffect* effect();
    bp::object& raw_effect();

    const std::unique_ptr<RegionMask>& mask() const;

    const std::vector<glm::vec3>& aabb();
    const std::vector<glm::vec3>& contour();

    const std::string& qml_path() const;

    void move();

    const glm::mat3x3& transform() const;

    uint8_t on_enter(PySIEffect* other);
    uint8_t on_continuous(PySIEffect* other);
    uint8_t on_leave(PySIEffect* other);

    Q_SIGNAL void LINK_SIGNAL(const std::string& uuid_event, const std::string& uuid_sender, const std::string& source_cap, const bp::object& args);
    Q_SLOT void LINK_SLOT(const std::string& uuid_event, const std::string& uuid_sender, const std::string& source_cap, const bp::object& args);
    Q_SLOT void REGION_DATA_CHANGED_SLOT(const QMap<QString, QVariant>& data);

    void register_link_event(const std::string& uuid, const std::string& attribute);
    void register_link_event(const std::tuple<std::string, std::string>& link_event);

    bool is_link_event_registered(const std::string& uuid, const std::string& attribute);
    bool is_link_event_registered(const std::tuple<std::string, std::string>& link_event);

    const std::string& name() const;
    const glm::vec4& color() const;
    const uint16_t type() const;
    const uint32_t width() const;
    const uint32_t height() const;

    uint8_t handle_collision_event(const std::string& function_name, PySIEffect* colliding_effect);

    void update();
    const QMap<QString, QVariant>& data() const;

    const int32_t last_delta_x() const;
    const int32_t last_delta_y() const;

    bool is_new();
    void set_is_new(bool toggle);

    int32_t x();
    int32_t y();

    std::vector<int>& grid_nodes();
    glm::ivec4& grid_bounds();

private:
    bool d_is_new = true;

    void process_canvas_specifics();
    void process_linking_relationships();

    PySIEffect* d_py_effect;
    std::shared_ptr<bp::object> d_effect;

    RingBuffer<std::tuple<std::string, std::string>> d_link_events;

    std::vector<int> d_grid_nodes;
    glm::ivec4 d_grid_bounds;

    std::unique_ptr<RegionMask> uprm;
    std::unique_ptr<RegionTransform> uprt;
    bool d_is_transformed;

    int32_t d_last_x;
    int32_t d_last_y;
    int32_t d_last_delta_x;
    int32_t d_last_delta_y;
};


#endif //SITEST_REGION_HPP
