

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

namespace bp = boost::python;

class PySIEffect: public SIObject
{ PYSI
public:
    PySIEffect(const std::vector<glm::vec3>& contour, const std::string& uuid, const std::string& tex_path, const bp::dict& kwargs);

    void __set_data__(const std::string& key, const bp::object& value, const uint32_t type, const bp::dict& data_kwargs);
    void __embed_file_standard_appliation_into_context__(const std::string& uuid, const std::string& path);
    void __destroy_embedded_file_standard_appliation_in_context__(const std::string& uuid);
    void __signal_deletion__();
    void __signal_deletion_by_uuid__(const std::string& uuid);
    void __assign_effect__(const std::string& sender, const std::string& effect_name, const std::string& effect_display_name, bp::dict& kwargs);
    void __emit_linking_action__(const std::string& sender, const std::string& linking_action, const bp::object& args);

    void __create_region__(const std::vector<glm::vec3>& contour, const std::string& name, bool as_selector, bp::dict& kwargs);
    void __create_region__(const bp::list& contour, const std::string& name, bool as_selector, bp::dict& kwargs);
    void __create_region__(const bp::list& contour, int effect_type, bp::dict& kwargs);
    void __create_region__(const bp::object& contour, const bp::dict& qml);
    bp::list __current_regions__();
    bp::list __excluded_plugins__();

    bp::dict __qml_data_keys_and_types__();

    void __update_transform__(int32_t delta_x, int32_t delta_y);

    bp::list __logger_messages__();

    bp::object __data__(const std::string& key, const uint32_t type);

    bp::tuple __context_dimensions__();

    std::vector<std::string> __available_plugins_by_name__();

    std::vector<glm::vec3> get_shape();
    void set_shape(const std::vector<glm::vec3>& shape);

    float d_x = 0;
    float d_y = 0;
    int32_t d_width = 0;
    int32_t d_height = 0;
    uint32_t d_effect_type = SI_TYPE_CUSTOM;
    int32_t d_transform_x = 0;
    int32_t d_transform_y = 0;
    const int32_t x() const;
    const int32_t y() const;
    const int32_t width() const;
    const int32_t height() const;
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

    bool d_recompute_mask = false;
    bool d_with_border = false;
    bool d_visible = true;

    float mouse_wheel_angle_degrees = 0.0;
    float mouse_wheel_angle_px = 0.0;

    void set_mouse_pressed_capability(uint32_t btn, bool active);
    bool has_mouse_pressed_capability(uint32_t btn);

    bool d_flagged_for_deletion = false;
    bool is_flagged_for_deletion();
    bool is_border_present();
    bool visible();


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

    std::unordered_map<std::string, bp::object> d_cap_link_emit;
    std::unordered_map<std::string, bp::object>& attr_link_emit();

    std::unordered_map<std::string, std::unordered_map<std::string, bp::object>> d_cap_link_recv;
    std::unordered_map<std::string, std::unordered_map<std::string, bp::object>> d_cap_collision_emit;
    std::unordered_map<std::string, std::unordered_map<std::string, bp::object>> d_cap_collision_recv;
    std::unordered_map<std::string, std::unordered_map<std::string, bp::object>>& attr_link_recv();
    std::unordered_map<std::string, std::unordered_map<std::string, bp::object>>& cap_collision_emit();
    std::unordered_map<std::string, std::unordered_map<std::string, bp::object>>& cap_collision_recv();

    std::unordered_map<std::string, std::vector<glm::vec3>> d_partial_regions;
    std::unordered_map<std::string, std::vector<glm::vec3>>& partial_region_contours();

    void set_data(const QMap<QString, QVariant>& data);
    const QMap<QString, QVariant>& data();
    bool d_data_changed;

private:
    QMap<QString, QVariant> d_data;
};


#endif //PYSI_SUPEREFFECT_HPP
