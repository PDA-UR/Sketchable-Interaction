

#ifndef PYSI_SUPEREFFECT_HPP
#define PYSI_SUPEREFFECT_HPP

#include <memory>
#include <boost/python.hpp>
#include <sigrun/util/Util.hpp>
#include <map>
#include <glm/glm.hpp>
#include <debug/Print.hpp>

namespace bp = boost::python;

class IterableConverter
{
public:
    /// @note Registers converter from a python interable type to the
    ///       provided type.
    template<typename Container>
    IterableConverter &from_python();

    /// @brief Check if PyObject is iterable.
    static void* convertible(PyObject* object);

    /// @brief Convert iterable PyObject to C++ container type.
    ///
    /// Container Concept requirements:
    ///
    ///   * Container::value_type is CopyConstructable.
    ///   * Container can be constructed and populated with two iterators.
    ///     I.e. Container(begin, end)
    template<typename Container>
    static void construct(PyObject* object, bp::converter::rvalue_from_python_stage1_data* data);
};

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
    enum EffectType
    {
        SI_CANVAS = 0,
        SI_CURSOR = 1,
        SI_MOUSE_CURSOR = 2,
        SI_CUSTOM = 1000
    };

    void __set_angle_degrees__(float angle)
    {
        d_angle_deg = angle;
        d_angle_radians = TO_RADIANS(angle);
    }

    void __set_angle_radians__(float angle)
    {
        d_angle_radians = angle;
        d_angle_deg = TO_DEGREES(angle);
    }

    float __angle_radians__()
    {
        return d_angle_radians;
    }

    float __angle_degrees__()
    {
        return d_angle_deg;
    }

    void __set_x__(float x)
    {
       d_x = (int) std::round(x);
    }

    void __set_y__(float y)
    {
        d_y = (int) std::round(y);
    }

    int __x__()
    {
        return d_x;
    }

    int __y__()
    {
        return d_y;
    }

    int x()
    {
        return __x__();
    }

    int y()
    {
        return __y__();
    }

    void __set_color__(const std::vector<int>& rgba)
    {
        d_color = glm::vec4(rgba[0], rgba[1], rgba[2], rgba[3]);
    }

    const std::vector<int> __color__() const
    {
        return std::vector<int> {(int) d_color.r, (int) d_color.g, (int) d_color.b, (int) d_color.a};
    }

    void __set_scale__(float factor)
    {
        d_scale = factor;
    }

    const float __scale__() const
    {
        return d_scale;
    }

    void __set_link_emit__(const bp::dict& dict)
    {
        d_cap_link_emit.clear();

        const bp::list& keys = dict.keys();

        for(int i = 0; i < bp::len(keys); ++i)
            d_cap_link_emit.insert({bp::extract<std::string>(keys[i]), dict[keys[i]]});
    }

    const bp::dict __link_emit__() const
    {
        bp::dict ret;

        for(auto& [key, value]: d_cap_link_emit)
            ret[key] = *value;

        return ret;
    }

    void __set_link_recv__(const bp::dict& dict)
    {
        d_cap_link_recv.clear();

        const bp::list& keys = dict.keys();

        for(int i = 0; i < bp::len(keys); ++i)
        {
            std::map<std::string, bp::object> map;

            const bp::dict &inner_dict = bp::extract<bp::dict>(dict[keys[i]]);
            const bp::list &inner_keys = inner_dict.keys();

            for (int k = 0; k < bp::len(inner_keys); ++k)
                map.insert({bp::extract<std::string>(inner_keys[k]), inner_dict[inner_keys[k]]});

            d_cap_link_recv.insert({bp::extract<std::string>(keys[i]), map});
        }
    }

    const bp::dict __link_recv__() const
    {
        bp::dict ret;

        for(auto& [key, value]: d_cap_link_recv)
        {
            bp::dict temp;

            for(auto& [key2, value2]: value)
                temp[key2] = value2;

            ret[key] = temp;
        }

        return ret;
    }

    std::map<std::string, bp::object>& attr_link_emit()
    {
        return d_cap_link_emit;
    }

    std::map<std::string, std::map<std::string, bp::object>>& attr_link_recv()
    {
        return d_cap_link_recv;
    }

    std::map<std::string, std::map<std::string, bp::object>>& cap_collision_emit()
    {
        return d_cap_collision_emit;
    }

    std::map<std::string, std::map<std::string, bp::object>>& cap_collision_recv()
    {
        return d_cap_collision_recv;
    }

    void __set_collision_emit__(const bp::dict& dict)
    {
        d_cap_collision_emit.clear();

        const bp::list& keys = dict.keys();

        for(int i = 0; i < bp::len(keys); ++i)
        {
            std::map<std::string, bp::object> map;

            const bp::dict &inner_dict = bp::extract<bp::dict>(dict[keys[i]]);
            const bp::list &inner_keys = inner_dict.keys();

            for (int k = 0; k < bp::len(inner_keys); ++k)
                map.insert({bp::extract<std::string>(inner_keys[k]), inner_dict[inner_keys[k]]});

            d_cap_collision_emit.insert({bp::extract<std::string>(keys[i]), map});
        }
    }

    const bp::dict __collision_emit__() const
    {
        bp::dict ret;

        for(auto& [key, value]: d_cap_collision_emit)
        {
            bp::dict temp;

            for(auto& [key2, value2]: value)
                temp[key2] = value2;

            ret[key] = temp;
        }

        return ret;
    }

    void __set_collision_recv__(const bp::dict& dict)
    {
        d_cap_collision_recv.clear();

        const bp::list& keys = dict.keys();

        for(int i = 0; i < bp::len(keys); ++i)
        {
            std::map<std::string, bp::object> map;

            const bp::dict &inner_dict = bp::extract<bp::dict>(dict[keys[i]]);
            const bp::list &inner_keys = inner_dict.keys();

            for (int k = 0; k < bp::len(inner_keys); ++k)
                map.insert({bp::extract<std::string>(inner_keys[k]), inner_dict[inner_keys[k]]});

            d_cap_collision_recv.insert({bp::extract<std::string>(keys[i]), map});
        }
    }

    const bp::dict __collision_recv__() const
    {
        bp::dict ret;

        for(auto& [key, value]: d_cap_collision_recv)
        {
            bp::dict temp;

            for(auto& [key2, value2]: value)
                temp[key2] = value2;

            ret[key] = temp;
        }

        return ret;
    }

    void __set__name__(const std::string& name)
    {
        d_name = name;
    }

    void __set__texture_path__(const std::string& text_path)
    {
        d_texture_path = text_path;
    }

    void __set__source__(const std::string& source)
    {
        d_source = source;
    }

    void __set_effect_type__(int type)
    {
        d_effect_type = (PySIEffect::EffectType) type;
    }

    const std::string __name__() const
    {
        return d_name;
    }

    const std::string __texture_path__() const
    {
        return d_texture_path;
    }

    const std::string __source__() const
    {
        return d_source;
    }

    const int __effect_type__() const
    {
        return d_effect_type;
    }

    const std::string& name() const
    {
        return d_name;
    }

    const std::string& texture_path() const
    {
        return d_texture_path;
    }

    const std::string& source() const
    {
        return d_source;
    }

    const int effect_type() const
    {
        return d_effect_type;
    }

    bool __is_left_mouse_clicked()
    {
        return d_is_left_mouse_clicked;
    }

    void __set_left_mouse_clicked__(bool active)
    {
        d_is_left_mouse_clicked = active;
    }

    bool __is_right_mouse_clicked()
    {
        return d_is_right_mouse_clicked;
    }

    void __set_right_mouse_clicked__(bool active)
    {
        d_is_right_mouse_clicked = active;
    }

    bool __is_middle_mouse_clicked()
    {
        return d_is_middle_mouse_clicked;
    }

    void __set_middle_mouse_clicked__(bool active)
    {
        d_is_middle_mouse_clicked = active;
    }

    bool has_mouse_pressed_capability(int btn)
    {
        switch (btn)
        {
            case 0: // left mouse button
                return d_is_left_mouse_clicked;
            case 1: // right mouse button
                return d_is_right_mouse_clicked;
            case 2: // middle mouse button
                return d_is_middle_mouse_clicked;
            default:
                return false;
        }
    }

    void set_mouse_pressed_capability(int btn, bool active)
    {
        switch (btn)
        {
            case 0: // left mouse button
                __set_left_mouse_clicked__(active);
            break;
            case 1: // right mouse button
                __set_right_mouse_clicked__(active);
                break;
            case 2: // middle mouse button
                __set_middle_mouse_clicked__(active);
                break;
            default:
                break;
        }
    }

    void __register_region__(const std::string& sender_id)
    {
        d_regions_marked_for_registration.push_back(sender_id);
    }

    std::vector<std::string>& regions_for_registration()
    {
        return d_regions_marked_for_registration;
    }

    void __set_uuid__(const std::string& uuid)
    {
        d_uuid = uuid;
    }

    const std::string __uuid__() const
    {
        return d_uuid;
    }

    void __set_regions_for_registration__(const std::vector<std::string>& candidates)
    {
        d_regions_marked_for_registration = candidates;
    }

    std::vector<std::string> __regions_for_registration__()
    {
        return d_regions_marked_for_registration;
    }

    void __add_point_to_partial_region__(const std::string& sender_id, const std::vector<float>& point)
    {
        if(d_partial_regions_py.find(sender_id) == d_partial_regions_py.end())
            d_partial_regions_py.insert({sender_id, std::vector<std::vector<float>>()});

        d_partial_regions_py[sender_id].emplace_back(point);
    }

    std::map<std::string, std::vector<glm::vec3>>& partial_region_contours()
    {
        d_partial_regions.clear();

        for(auto& [key, value]: d_partial_regions_py)
        {
            d_partial_regions[key] = std::vector<glm::vec3>();

            for(auto p: value)
            {
                d_partial_regions[key].emplace_back(p[0], p[1], 1);
            }
        }

        return d_partial_regions;
    }

    void __set_partial_regions__(const std::map<std::string, std::vector<std::vector<float>>>& partials)
    {
        d_partial_regions_py = partials;
    }

    std::map<std::string, std::vector<std::vector<float>>> __partial_regions__()
    {
        return d_partial_regions_py;
    }

    void __set_transformer_id__(const std::string& id)
    {
        d_transformer_id = id;
    }

    const std::string __transformer_id__() const
    {
        return d_transformer_id;
    }

    const std::string& transformer_id() const
    {
        return d_transformer_id;
    }

    void __register_link__(const std::string& sender, const std::string& sender_attrib, const std::string& recv, const std::string& recv_attrib)
    {
        d_link_relations.push_back(std::make_shared<LinkRelation>(sender, sender_attrib, recv, recv_attrib));
    }

    void __remove_link__(const std::string& sender, const std::string& sender_attrib, const std::string& recv, const std::string& recv_attrib)
    {

        for(int i = d_link_relations.size() - 1; i > -1; --i)
        {
            if(sender == d_link_relations[i]->sender &&
                sender_attrib == d_link_relations[i]->sender_attrib &&
                recv_attrib == d_link_relations[i]->recv_attrib)
            {
                d_link_relations.erase(d_link_relations.begin() + i);

                break;
            }
        }
    }

    std::vector<std::shared_ptr<LinkRelation>>& link_relations()
    {
        return d_link_relations;
    }

private:
    std::vector<std::shared_ptr<LinkRelation>> d_link_relations;

    int d_x = 0;
    int d_y = 0;

    float d_angle_deg = 0.0;
    float d_angle_radians = 0.0;

    float d_scale = 1.0f;

    glm::vec4 d_color = glm::vec4(255, 255, 255, 255);

    std::map<std::string, std::map<std::string, bp::object>> d_cap_collision_emit;
    std::map<std::string, std::map<std::string, bp::object>> d_cap_collision_recv;

    std::map<std::string, bp::object> d_cap_link_emit;
    std::map<std::string, std::map<std::string, bp::object>> d_cap_link_recv;

    std::map<std::string, std::vector<glm::vec3>> d_partial_regions;
    std::map<std::string, std::vector<std::vector<float>>> d_partial_regions_py;

    std::vector<std::string> d_regions_marked_for_registration;

    std::string d_name = "";
    std::string d_uuid = "";
    std::string d_texture_path = "";
    std::string d_source = "";
    std::string d_transformer_id = "";
    PySIEffect::EffectType d_effect_type = PySIEffect::EffectType::SI_CUSTOM;

    bool d_is_left_mouse_clicked = false;
    bool d_is_right_mouse_clicked = false;
    bool d_is_middle_mouse_clicked = false;
};


#endif //PYSI_SUPEREFFECT_HPP
