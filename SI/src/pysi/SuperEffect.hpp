

#ifndef PYSI_SUPEREFFECT_HPP
#define PYSI_SUPEREFFECT_HPP

#include <boost/python.hpp>
#include <sigrun/util/Util.hpp>
#include <map>
#include <glm/glm.hpp>

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
            d_cap_link_emit.insert({bp::extract<std::string>(keys[i]), std::make_shared<bp::object>(dict[keys[i]])});
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
            std::map<std::string, std::shared_ptr<bp::object>> map;

            const bp::dict &inner_dict = bp::extract<bp::dict>(dict[keys[i]]);
            const bp::list &inner_keys = inner_dict.keys();

            for (int k = 0; k < bp::len(inner_keys); ++k)
                map.insert({bp::extract<std::string>(inner_keys[k]), std::make_shared<bp::object>(inner_dict[inner_keys[k]])});

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

    std::map<std::string, std::shared_ptr<bp::object>>& attr_link_emit()
    {
        return d_cap_link_emit;
    }

    std::map<std::string, std::map<std::string, std::shared_ptr<bp::object>>>& attr_link_recv()
    {
        return d_cap_link_recv;
    }

    std::map<std::string, std::map<std::string, std::shared_ptr<bp::object>>>& cap_collision_emit()
    {
        return d_cap_collision_emit;
    }

    std::map<std::string, std::map<std::string, std::shared_ptr<bp::object>>>& cap_collision_recv()
    {
        return d_cap_collision_recv;
    }

    void __set_collision_emit__(const bp::dict& dict)
    {
        d_cap_collision_emit.clear();

        const bp::list& keys = dict.keys();

        for(int i = 0; i < bp::len(keys); ++i)
        {
            std::map<std::string, std::shared_ptr<bp::object>> map;

            const bp::dict &inner_dict = bp::extract<bp::dict>(dict[keys[i]]);
            const bp::list &inner_keys = inner_dict.keys();

            for (int k = 0; k < bp::len(inner_keys); ++k)
                map.insert({bp::extract<std::string>(inner_keys[k]), std::make_shared<bp::object>(inner_dict[inner_keys[k]])});

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
            std::map<std::string, std::shared_ptr<bp::object>> map;

            const bp::dict &inner_dict = bp::extract<bp::dict>(dict[keys[i]]);
            const bp::list &inner_keys = inner_dict.keys();

            for (int k = 0; k < bp::len(inner_keys); ++k)
                map.insert({bp::extract<std::string>(inner_keys[k]), std::make_shared<bp::object>(inner_dict[inner_keys[k]])});

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

private:
    int d_x = 0;
    int d_y = 0;

    float d_angle_deg = 0.0;
    float d_angle_radians = 0.0;

    float d_scale = 1.0f;

    glm::vec4 d_color = glm::vec4(255, 255, 255, 255);

    std::map<std::string, std::map<std::string, std::shared_ptr<bp::object>>> d_cap_collision_emit;
    std::map<std::string, std::map<std::string, std::shared_ptr<bp::object>>> d_cap_collision_recv;

    std::map<std::string, std::shared_ptr<bp::object>> d_cap_link_emit;
    std::map<std::string, std::map<std::string, std::shared_ptr<bp::object>>> d_cap_link_recv;

    std::string d_name;
    std::string d_texture_path;
    std::string d_source;
    PySIEffect::EffectType d_effect_type;
};


#endif //PYSI_SUPEREFFECT_HPP
