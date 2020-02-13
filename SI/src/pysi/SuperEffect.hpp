

#ifndef PYSI_SUPEREFFECT_HPP
#define PYSI_SUPEREFFECT_HPP

#include <memory>
#include <boost/python.hpp>
#include <sigrun/util/Util.hpp>
#include <map>
#include <glm/glm.hpp>
#include <debug/Print.hpp>
#include <sigrun/SITypes.hpp>
#include <QVariant>
#include <QMap>

namespace bp = boost::python;

struct LinkRelation
{
    LinkRelation(const std::string& _sender, const std::string& _sender_attrib, const std::string& _recv, const std::string& _recv_attrib):
            sender(_sender),
            sender_attrib(_sender_attrib),
            recv(_recv),
            recv_attrib(_recv_attrib)
    {}

    std::string sender;
    std::string sender_attrib;
    std::string recv;
    std::string recv_attrib;
};

class PySIEffect
{
public:
    void __set_angle_degrees__(float angle);
    void __set_angle_radians__(float angle);
    float __angle_radians__();
    float __angle_degrees__();
    void __set_x__(float x);
    void __set_y__(float y);
    void __set_width__(float width);
    void __set_height__(float height);
    int __x__();
    int __y__();
    int __width__();
    int __height__();
    int x();
    int y();
    int width();
    int height();
    const glm::vec4& color() const;
    void __set_scale__(float factor);
    const float __scale__() const;
    std::map<std::string, bp::object>& attr_link_emit();
    std::map<std::string, std::map<std::string, bp::object>>& attr_link_recv();
    std::map<std::string, std::map<std::string, bp::object>>& cap_collision_emit();
    std::map<std::string, std::map<std::string, bp::object>>& cap_collision_recv();
    void __set__name__(const std::string& name);
    void __set_qml_path__(const std::string& path);
    void __set_source__(const std::string& source);
    void __set_effect_type__(int type);
    const std::string __name__() const;
    const std::string __qml_path__() const;
    const std::string __source__() const;
    const int __effect_type__() const;
    const std::string& name() const;
    const std::string& qml_path() const;
    const std::string& source() const;
    const int effect_type() const;
    bool __is_left_mouse_clicked();
    void __set_left_mouse_clicked__(bool active);
    bool __is_right_mouse_clicked();
    void __set_right_mouse_clicked__(bool active);
    bool __is_middle_mouse_clicked();
    void __set_middle_mouse_clicked__(bool active);
    bool has_mouse_pressed_capability(int btn);
    void set_mouse_pressed_capability(int btn, bool active);
    void __register_region__(const std::string& sender_id);
    std::vector<std::string>& regions_for_registration();
    void __set_uuid__(const std::string& uuid);
    const std::string __uuid__() const;
    std::map<std::string, std::vector<glm::vec3>>& partial_region_contours();
    void __register_link__(const std::string& sender, const std::string& sender_attrib, const std::string& recv, const std::string& recv_attrib);
    void __remove_link__(const std::string& sender, const std::string& sender_attrib, const std::string& recv, const std::string& recv_attrib);
    std::vector<std::shared_ptr<LinkRelation>>& link_relations();

    void __set_data__(const std::string& key, const bp::object& value, const int type);
    const QMap<QString, QVariant>& data();
    bool has_data_changed();

    // migrated to new system
    std::map<std::string, std::vector<glm::vec3>> d_partial_regions;
    std::map<std::string, std::map<std::string, bp::object>> d_cap_collision_emit;
    std::map<std::string, std::map<std::string, bp::object>> d_cap_collision_recv;
    std::map<std::string, bp::object> d_cap_link_emit;
    std::map<std::string, std::map<std::string, bp::object>> d_cap_link_recv;
    std::vector<std::string> d_regions_marked_for_registration;
    glm::vec4 d_color;

    // pending
    std::vector<std::shared_ptr<LinkRelation>> d_link_relations;


private:
    int d_x = 0;
    int d_y = 0;
    int d_width = 0;
    int d_height = 0;
    int d_effect_type = SI_TYPE_CUSTOM;

    float d_angle_deg = 0.0;
    float d_angle_radians = 0.0;
    float d_scale = 1.0f;

    std::string d_name = "";
    std::string d_uuid = "";
    std::string d_qml_path = "";
    std::string d_source = "";

    QMap<QString, QVariant> d_data;
    bool d_data_changed;

    bool d_is_left_mouse_clicked = false;
    bool d_is_right_mouse_clicked = false;
    bool d_is_middle_mouse_clicked = false;
};


#endif //PYSI_SUPEREFFECT_HPP
