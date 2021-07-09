
#ifndef SITEST_PICKLESUITS_HPP
#define SITEST_PICKLESUITS_HPP

#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/class.hpp>
#include <boost/python/tuple.hpp>
#include <boost/python/dict.hpp>
#include <boost/python/extract.hpp>
#include <boost/python/back_reference.hpp>
#include <pysi/PySIEffect.hpp>
#include <debug/Print.hpp>

namespace bp = boost::python;

class EffectPickleSuit: public bp::pickle_suite
{
public:
    static bp::tuple getstate(bp::object o)
    {
        PySIEffect e = bp::extract<PySIEffect>(o);

        bp::tuple t = bp::make_tuple(o.attr("__dict__"),
                                     e.get_shape(),
                                     e.aabb(),
                                     e.d_recompute_mask,
                                     e.partial_region_contours(),
                                     e.regions_for_registration(),
                                     e.cap_collision_emit(),
                                     e.cap_collision_recv(),
                                     e.d_cap_link_emit,
                                     e.d_cap_link_recv,
                                     e.color(),
                                     e.x(),
                                     e.y(),
                                     e.d_transform_x);

        bp::tuple t2 = bp::make_tuple(e.d_transform_y,
                                      e.width(),
                                      e.height(),
                                      e.angle_degrees(),
                                      e.scale(),
                                      e.name(),
                                      e.uuid(),
                                      e.source(),
                                      e.qml_path(),
                                      e.effect_type(),
                                      e.d_is_left_mouse_clicked,
                                      e.d_is_right_mouse_clicked,
                                      e.d_is_middle_mouse_clicked,
                                      e.link_relations(),
                                      e.has_data_changed());

        bp::tuple t3 = bp::make_tuple(e.mouse_wheel_angle_px,
                                      e.mouse_wheel_angle_degrees,
                                      e.d_with_border);

        bp::tuple ret = bp::extract<bp::tuple>(t + t2 + t3);

        return ret;
    }

    static void setstate(bp::object o, bp::tuple state)
    {
        PySIEffect& e = bp::extract<PySIEffect&>(o);

        bp::dict d = bp::extract<bp::dict>(o.attr("__dict__"));
        d.update(state[0]);

        auto shape = bp::extract<std::vector<glm::vec3>>(state[1]);
        e.set_shape(shape);

        e.d_recompute_mask = bp::extract<bool>(state[3]);
        e.d_partial_regions = bp::extract<std::unordered_map<std::string, std::vector<glm::vec3>>>(state[4]);
        e.d_regions_marked_for_registration = bp::extract<std::vector<std::string>>(state[5]);
        e.d_cap_collision_emit = bp::extract<std::unordered_map<std::string, std::unordered_map<std::string, bp::object>>>(state[6]);
        e.d_cap_collision_recv = bp::extract<std::unordered_map<std::string, std::unordered_map<std::string, bp::object>>>(state[7]);
        e.d_cap_link_emit = bp::extract<std::unordered_map<std::string, bp::object>>(state[8]);
        e.d_cap_link_recv = bp::extract<std::unordered_map<std::string, std::unordered_map<std::string, bp::object>>>(state[9]);
        e.d_color = bp::extract<glm::vec4>(state[10]);
        e.d_x = bp::extract<float>(state[11]);
        e.d_y = bp::extract<float>(state[12]);
        e.d_transform_x = bp::extract<int32_t>(state[13]);
        e.d_transform_y = bp::extract<int32_t>(state[14]);
        e.d_width = bp::extract<uint32_t>(state[15]);
        e.d_height = bp::extract<uint32_t>(state[16]);
        e.d_angle_deg = bp::extract<float>(state[17]);
        e.d_scale = bp::extract<float>(state[18]);
        e.d_name = bp::extract<std::string>(state[19]);
        e.d_uuid = bp::extract<std::string>(state[20]);
        e.d_source = bp::extract<std::string>(state[21]);
        e.d_qml_path = bp::extract<std::string>(state[22]);
        e.d_effect_type = bp::extract<uint32_t>(state[23]);
        e.d_is_left_mouse_clicked = bp::extract<bool>(state[24]);
        e.d_is_right_mouse_clicked = bp::extract<bool>(state[25]);
        e.d_is_middle_mouse_clicked = bp::extract<bool>(state[26]);
        e.d_link_relations = bp::extract<std::vector<LinkCandidate>>(state[27]);
        e.d_data_changed = bp::extract<bool>(state[28]);
        e.mouse_wheel_angle_px = bp::extract<float>(state[29]);
        e.mouse_wheel_angle_degrees = bp::extract<float>(state[30]);
        e.d_with_border = bp::extract<bool>(state[31]);
    }

    static bool getstate_manages_dict()
    {
        return true;
    }
};

class Point2PickleSuit: public bp::pickle_suite
{
public:
    static bp::tuple getinitargs(glm::vec2& p)
    {
        return bp::make_tuple(p.x, p.y);
    }
};

class Point3PickleSuit: public bp::pickle_suite
{
public:
    static bp::tuple getinitargs(glm::vec3& p)
    {
        return bp::make_tuple(p.x, p.y, p.z);
    }
};

class ColorPickleSuit: public bp::pickle_suite
{
public:
    static bp::tuple getinitargs(glm::vec4& c)
    {
        return bp::make_tuple(c.r, c.g, c.b, c.a);
    }
};

class LinkRelationPickleSuit: public bp::pickle_suite
{
public:
    static bp::tuple getinitargs(LinkCandidate& lc)
    {
        return bp::make_tuple(lc.sender, lc.sender_attrib, lc.recv, lc.recv_attrib);
    }
};

class PointVectorPickleSuite: public bp::pickle_suite
{
public:
    static bp::tuple getinitargs(std::vector<glm::vec3>& v)
    {
        bp::list l;

        for(auto& p: v)
        {
            bp::list t;

            t.append(p.x);
            t.append(p.y);

            l.append(t);
        }

        return bp::make_tuple(l);
    }
};

class LinkRelationVectorPickleSuite: public bp::pickle_suite
{
public:
    static bp::tuple getinitargs(std::vector<LinkCandidate>& v)
    {
        bp::list l;

        for(auto& lc: v)
        {
            bp::list t;
            t.append(lc.sender);
            t.append(lc.sender_attrib);
            t.append(lc.recv);
            t.append(lc.recv_attrib);

            l.append(t);
        }

        return bp::make_tuple(l);
    }
};

class StringVectorPickleSuite: public bp::pickle_suite
{
public:
    static bp::tuple getinitargs(std::vector<std::string>& v)
    {
        bp::list l;

        for(auto& s: v)
            l.append(s);

        return bp::make_tuple(l);
    }
};

class PartialContourPickleSuite: public bp::pickle_suite
{
public:
    static bp::tuple getinitargs(std::unordered_map<std::string, std::vector<glm::vec3>>& m)
    {
        bp::dict d;

        for(auto& [k, v]: m)
        {
            bp::list l;

            for(auto& e: v)
            {
                l.append(e.x);
                l.append(e.y);
            }

            d[k] = l;
        }

        return bp::make_tuple(d);
    }
};

class String2FunctionMapPickleSuite: public bp::pickle_suite
{
public:
    static bp::tuple getinitargs(std::unordered_map<std::string, bp::object>& m)
    {
        bp::dict d;

        for(auto& [k, v]: m)
            d[k] = v;

        return bp::make_tuple(d);
    }
};

class String2String2FunctionMapMapPickleSuite: public bp::pickle_suite
{
public:
    static bp::tuple getinitargs(std::unordered_map<std::string, std::unordered_map<std::string, bp::object>>& m)
    {
        bp::dict d;

        for(auto& [k, v]: m)
        {
            bp::dict d2;

            for(auto& [k2, v2]: v)
                d2[k2] = v2;

            d[k] = d2;
        }

        return bp::make_tuple(d);
    }
};

#endif //SITEST_PICKLESUITS_HPP
