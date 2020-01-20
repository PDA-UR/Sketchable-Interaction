

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

namespace bp = boost::python;

Q_DECLARE_METATYPE (bp::object)
Q_DECLARE_METATYPE (bp::list)


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

    Q_SIGNAL void LINK_SIGNAL(const std::string& uuid, const std::string& source_cap, const bp::list& py_list);
    Q_SLOT void LINK_SLOT(const std::string& uuid, const std::string& source_cap, const bp::list& py_list);

    void register_link_event(const std::string& uuid, const std::string& attribute);
    void register_link_event(const std::tuple<std::string, std::string>& link_event);

    bool is_link_event_registered(const std::string& uuid, const std::string& attribute);
    bool is_link_event_registered(const std::tuple<std::string, std::string>& link_event);

    void set_name(const std::string& name);
    const std::string& name() const;

private:
    std::vector<glm::vec3> d_contour;
    std::vector<glm::vec3> d_aabb;

//    std::vector<std::tuple<std::string, std::string>> d_link_events;

    RingBuffer<std::tuple<std::string, std::string>> d_link_events;

    std::unique_ptr<RegionMask> uprm;
    std::unique_ptr<RegionTransform> uprt;
    std::unique_ptr<PythonInvoker> uppi;
    std::shared_ptr<bp::object> d_effect;
    std::string d_uuid;
    bool d_is_transformed;

    std::string d_texture_path_default;
    std::string d_name;

    std::vector<std::string> d_attributes_emit;
    std::unordered_map<std::string, std::vector<std::string>> d_attributes_recv;
};


#endif //SITEST_REGION_HPP
