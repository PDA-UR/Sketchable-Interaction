

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
#include <sigrun/SIObject.hpp>

#include <sigrun/log/Log.hpp>

#define PYSI_DEBUG(what) Log::log("PySI", what, Log::LOG_LEVEL::DEBUG_LEVEL, "PySIEffect",__FILENAME__, __FUNCTION__, std::to_string(__LINE__))
#define PYSI_INFO(what) Log::log("PySI", what, Log::LOG_LEVEL::INFO_LEVEL, "PySIEffect",__FILENAME__, __FUNCTION__, std::to_string(__LINE__))

#define REQUIRES_NEW_SHAPE 0b01
#define REQUIRES_RESAMPLE 0b10

namespace bp = boost::python;

struct LinkRelation
{
    LinkRelation(const std::string& _sender, const std::string& _sender_attrib, const std::string& _recv, const std::string& _recv_attrib):
            sender(_sender),
            sender_attrib(_sender_attrib),
            recv(_recv),
            recv_attrib(_recv_attrib)
    {}

    const bool operator ==(const LinkRelation& other) const
    {
        return sender == other.sender && sender_attrib == other.sender_attrib && recv == other.recv && recv_attrib == other.recv_attrib;
    }

    std::string sender;
    std::string sender_attrib;
    std::string recv;
    std::string recv_attrib;
};

class PySIEffect
{
public:
    void init(const std::vector<glm::vec3>& contour, const std::vector<glm::vec3>& aabb, const std::string& uuid, const bp::dict& kwargs);
    void __add_data__(const std::string& key, const bp::object& value, const int type);
    void notify_shape_changed(bool resample=false);
    void __show_folder_contents__(const std::vector<std::string>& page_contents, const std::string& uuid, const bool with_btns=false);
    void __embed_file_standard_appliation_into_context__(const std::string& uuid, const std::string& path);
    void __destroy_embedded_file_standard_appliation_in_context__(const std::string& uuid);
    void __spawn_region__(const std::vector<glm::vec3>& contour, int type);
    void signal_deletion();

    float d_x = 0;
    float d_y = 0;
    int d_width = 0;
    int d_height = 0;
    int d_effect_type = SI_TYPE_CUSTOM;
    const int x() const;
    const int y() const;
    const int width() const;
    const int height() const;
    const int effect_type() const;

    float d_scale = 1.0f;
    float d_angle_deg = 0.0;
    const float scale() const;
    const float angle_degrees() const;

    std::string d_name = "";
    std::string d_uuid = "";
    std::string d_qml_path = "";
    std::string d_source = "";
    const std::string& name() const;
    const std::string& qml_path() const;
    const std::string& source() const;
    const std::string& uuid() const;

    bool d_is_left_mouse_clicked = false;
    bool d_is_right_mouse_clicked = false;
    bool d_is_middle_mouse_clicked = false;

    float mouse_wheel_angle_degrees = 0.0;
    float mouse_wheel_angle_px = 0.0;

    void set_mouse_pressed_capability(int btn, bool active);
    bool has_mouse_pressed_capability(int btn);

    bool d_flagged_for_deletion = false;
    bool is_flagged_for_deletion();

    bool d_has_shape_changed = false;
    int has_shape_changed();

    const bool has_data_changed() const;

    std::vector<std::string> d_regions_marked_for_registration;
    std::vector<std::string>& regions_for_registration();

    std::vector<LinkRelation> d_link_relations;
    std::vector<LinkRelation>& link_relations();

    std::vector<glm::vec3> d_contour;
    std::vector<glm::vec3> d_aabb;

    std::vector<glm::vec3>& contour();
    std::vector<glm::vec3>& aabb();

    glm::vec4 d_color;
    const glm::vec4& color() const;

    std::map<std::string, bp::object> d_cap_link_emit;
    std::map<std::string, bp::object>& attr_link_emit();

    std::map<std::string, std::map<std::string, bp::object>> d_cap_link_recv;
    std::map<std::string, std::map<std::string, bp::object>> d_cap_collision_emit;
    std::map<std::string, std::map<std::string, bp::object>> d_cap_collision_recv;
    std::map<std::string, std::map<std::string, bp::object>>& attr_link_recv();
    std::map<std::string, std::map<std::string, bp::object>>& cap_collision_emit();
    std::map<std::string, std::map<std::string, bp::object>>& cap_collision_recv();

    std::map<std::string, std::vector<glm::vec3>> d_partial_regions;
    std::map<std::string, std::vector<glm::vec3>>& partial_region_contours();

    const QMap<QString, QVariant>& data();
    bool d_data_changed;
    bool d_require_resample;

private:
    QMap<QString, QVariant> d_data;
};


#endif //PYSI_SUPEREFFECT_HPP
