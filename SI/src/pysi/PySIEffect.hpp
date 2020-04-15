

#ifndef PYSI_SUPEREFFECT_HPP
#define PYSI_SUPEREFFECT_HPP

#include <memory>
#include <boost/python.hpp>
#include <sigrun/util/Util.hpp>
#include <map>
#include <glm/glm.hpp>
#include <sigrun/SITypes.hpp>
#include <QVariant>
#include <QMap>
#include <sigrun/SIObject.hpp>
#include <sigrun/log/Log.hpp>
#include <sigrun/context/managers/helpers/linking/LinkCandidate.hpp>


#define PYSI_DEBUG(what) Log::log("PySI", what, Log::LOG_LEVEL::DEBUG_LEVEL, "PySIEffect",__FILENAME__, __FUNCTION__, std::to_string(__LINE__))
#define PYSI_INFO(what) Log::log("PySI", what, Log::LOG_LEVEL::INFO_LEVEL, "PySIEffect",__FILENAME__, __FUNCTION__, std::to_string(__LINE__))

#define REQUIRES_NEW_SHAPE 0b01
#define REQUIRES_RESAMPLE 0b10

namespace bp = boost::python;

class PySIEffect: public SIObject
{ PYSI
public:
    void init(const std::vector<glm::vec3>& contour, const std::vector<glm::vec3>& aabb, const std::string& uuid, const bp::dict& kwargs);
    void __add_data__(const std::string& key, const bp::object& value, const uint32_t type);
    void notify_shape_changed(bool resample=false);
    void __show_folder_contents__(const std::vector<std::string>& page_contents, const std::string& uuid, const bool with_btns=false);
    void __embed_file_standard_appliation_into_context__(const std::string& uuid, const std::string& path);
    void __destroy_embedded_file_standard_appliation_in_context__(const std::string& uuid);
    void signal_deletion();

    float d_x = 0;
    float d_y = 0;
    uint32_t d_width = 0;
    uint32_t d_height = 0;
    uint32_t d_effect_type = SI_TYPE_CUSTOM;
    const uint32_t x() const;
    const uint32_t y() const;
    const uint32_t width() const;
    const uint32_t height() const;
    const uint32_t effect_type() const;

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

    void set_mouse_pressed_capability(uint32_t btn, bool active);
    bool has_mouse_pressed_capability(uint32_t btn);

    bool d_flagged_for_deletion = false;
    bool is_flagged_for_deletion();

    bool d_has_shape_changed = false;
    uint32_t has_shape_changed();

    const bool has_data_changed() const;

    std::vector<std::string> d_regions_marked_for_registration;
    std::vector<std::string>& regions_for_registration();

    std::vector<LinkCandidate> d_link_relations;
    std::vector<LinkCandidate>& link_relations();

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
