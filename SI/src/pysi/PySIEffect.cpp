
#include "PySIEffect.hpp"
#include <sigrun/context/Capability.hpp>
#include "pysi/stl_container_exposure/MapExposure.hpp"
#include "pysi/stl_container_exposure/VectorExposure.hpp"

namespace bp = boost::python;

void PySIEffect::init(const std::vector<glm::vec3>& contour, const std::vector<glm::vec3>& aabb, const std::string& uuid)
{
    d_contour = contour;
    d_aabb = aabb;
    d_uuid = uuid;
    d_has_shape_changed = false;

    d_regions_marked_for_registration.reserve(10);
    d_link_relations.reserve(100);
    d_contour.reserve(64);
    d_aabb.reserve(4);
}

// has to be set to false from elsewhere (happens in Region.cpp, where value is used
void PySIEffect::notify_shape_changed()
{
    d_has_shape_changed = true;
}

bool PySIEffect::has_shape_changed()
{
    return d_has_shape_changed;
}

const int PySIEffect::x() const
{
    return d_x;
}

const int PySIEffect::y() const
{
    return d_y;
}

const glm::vec4& PySIEffect::color() const
{
    return d_color;
}

std::map<std::string, bp::object>& PySIEffect::attr_link_emit()
{
    return d_cap_link_emit;
}

std::map<std::string, std::map<std::string, bp::object>>& PySIEffect::attr_link_recv()
{
    return d_cap_link_recv;
}

std::map<std::string, std::map<std::string, bp::object>>& PySIEffect::cap_collision_emit()
{
    return d_cap_collision_emit;
}

std::map<std::string, std::map<std::string, bp::object>>& PySIEffect::cap_collision_recv()
{
    return d_cap_collision_recv;
}

const std::string& PySIEffect::name() const
{
    return d_name;
}

const std::string& PySIEffect::qml_path() const
{
    return d_qml_path;
}

const std::string& PySIEffect::source() const
{
    return d_source;
}

const int PySIEffect::effect_type() const
{
    return d_effect_type;
}

bool PySIEffect::has_mouse_pressed_capability(int btn)
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

void PySIEffect::set_mouse_pressed_capability(int btn, bool active)
{
    switch (btn)
    {
        case 0: // left mouse button
            d_is_left_mouse_clicked = active;
            break;
        case 1: // right mouse button
            d_is_right_mouse_clicked = active;
            break;
        case 2: // middle mouse button
            d_is_middle_mouse_clicked = active;
            break;
        default:
            break;
    }
}

std::vector<std::string>& PySIEffect::regions_for_registration()
{
    return d_regions_marked_for_registration;
}

const std::string& PySIEffect::uuid() const
{
    return d_uuid;
}

std::map<std::string, std::vector<glm::vec3>>& PySIEffect::partial_region_contours()
{
    return d_partial_regions;
}

const int PySIEffect::width() const
{
    return d_width;
}

const int PySIEffect::height() const
{
    return d_height;
}

const float PySIEffect::scale() const
{
    return d_scale;
}

const float PySIEffect::angle_degrees() const
{
    return d_angle_deg;
}

std::vector<LinkRelation>& PySIEffect::link_relations()
{
    return d_link_relations;
}

std::vector<glm::vec3> &PySIEffect::contour()
{
    return d_contour;
}

std::vector<glm::vec3> &PySIEffect::aabb()
{
    return d_aabb;
}

void PySIEffect::__add_data__(const std::string &key, const bp::object &value, const int type)
{
    QVariant qv;

    switch (type)
    {
        case SI_DATA_TYPE_INT:
            d_data[QString(key.c_str())] = QVariant( bp::extract<int>(value));
        break;

        case SI_DATA_TYPE_FLOAT:
            d_data[QString(key.c_str())] = QVariant(bp::extract<float>(value));
        break;

        case SI_DATA_TYPE_STRING:
            d_data[QString(key.c_str())] = QVariant(QString(bp::extract<char*>(value)));
        break;
    }

    d_data_changed = true;
}

const QMap<QString, QVariant> &PySIEffect::data()
{
    d_data_changed = false;

    return d_data;
}

const bool PySIEffect::has_data_changed() const
{
    return d_data_changed;
}

BOOST_PYTHON_MODULE(libPySI)
{
    bp::class_<Capability>("PySICapability")
        .add_static_property("__TEST1__", bp::make_getter(&Capability::__test1__))
        .add_static_property("__TEST2__", bp::make_getter(&Capability::__test2__))
        ;

    bp::scope the_scope = bp::class_<PySIEffect>("PySIEffect")
    ;

    bp::class_<glm::vec2>("Point2", bp::init<float, float>())
            .def_readwrite("x", &glm::vec2::x)
            .def_readwrite("y", &glm::vec2::y)

            .enable_pickling()
            ;

    bp::class_<glm::vec3>("Point3", bp::init<float, float, float>())
            .def_readwrite("x", &glm::vec3::x)
            .def_readwrite("y", &glm::vec3::y)
            .def_readwrite("z", &glm::vec3::z)

            .enable_pickling()
            ;

    bp::class_<glm::vec4>("Color", bp::init<float, float, float, float>())
            .def_readwrite("r", &glm::vec4::r)
            .def_readwrite("g", &glm::vec4::g)
            .def_readwrite("b", &glm::vec4::b)
            .def_readwrite("a", &glm::vec4::a)

            .enable_pickling()
            ;

    bp::class_<LinkRelation>("LinkRelation", bp::init<const std::string&, const std::string&, const std::string&, const std::string&>())
        .def_readwrite("sender", &LinkRelation::sender)
        .def_readwrite("sender_attrib", &LinkRelation::sender_attrib)
        .def_readwrite("recv", &LinkRelation::recv)
        .def_readwrite("recv_attrib", &LinkRelation::recv_attrib)

        .enable_pickling()
        ;

    create_vector<VectorExposureVec3, std::vector<glm::vec3>>("PointVector");
    create_vector<VectorExposureLinkRelation, std::vector<LinkRelation>>("LinkRelationVector");
    create_vector<VectorExposureString, std::vector<std::string>>("StringVector");
    create_map<MapExposurePartialContour, std::map<std::string, std::vector<glm::vec3>>>("PartialContour");
    create_map<MapExposureString2Function, std::map<std::string, bp::object>>("String2FunctionMap");
    create_map<MapExposureString2_String2FunctionMap_Map, std::map<std::string, std::map<std::string, bp::object>>>("String2_String2FunctionMap_Map");

    bp::class_<PySIEffect, boost::noncopyable>("PySIEffect", bp::init<>())
        .def("__init__", bp::make_constructor(&PySIEffect::init, bp::default_call_policies(), (bp::arg("shape")=std::vector<glm::vec3>(), bp::arg("aabb")=std::vector<glm::vec3>(), bp::arg("uuid")=std::string())))
        .def("add_data", &PySIEffect::__add_data__)
        .def("notify_shape_changed", &PySIEffect::notify_shape_changed)

        .def_readwrite("__partial_regions__", &PySIEffect::d_partial_regions)
        .def_readwrite("cap_emit", &PySIEffect::d_cap_collision_emit)
        .def_readwrite("cap_recv", &PySIEffect::d_cap_collision_recv)
        .def_readwrite("cap_link_emit", &PySIEffect::d_cap_link_emit)
        .def_readwrite("cap_link_recv", &PySIEffect::d_cap_link_recv)
        .def_readwrite("registered_regions", &PySIEffect::d_regions_marked_for_registration)
        .def_readwrite("color", &PySIEffect::d_color)
        .def_readwrite("x", &PySIEffect::d_x)
        .def_readwrite("y", &PySIEffect::d_y)
        .def_readwrite("width", &PySIEffect::d_width)
        .def_readwrite("height", &PySIEffect::d_height)
        .def_readwrite("angle_degrees", &PySIEffect::d_angle_deg)
        .def_readwrite("scale", &PySIEffect::d_scale)
        .def_readwrite("name", &PySIEffect::d_name)
        .def_readwrite("_uuid", &PySIEffect::d_uuid)
        .def_readwrite("source", &PySIEffect::d_source)
        .def_readwrite("qml_path", &PySIEffect::d_qml_path)
        .def_readwrite("region_type", &PySIEffect::d_effect_type)
        .def_readwrite("left_mouse_clicked", &PySIEffect::d_is_left_mouse_clicked)
        .def_readwrite("right_mouse_clicked", &PySIEffect::d_is_right_mouse_clicked)
        .def_readwrite("middle_mouse_clicked", &PySIEffect::d_is_middle_mouse_clicked)
        .def_readwrite("link_relations", &PySIEffect::d_link_relations)
        .def_readwrite("shape", &PySIEffect::d_contour)
        .def_readwrite("aabb", &PySIEffect::d_aabb)
        .def_readwrite("has_shape_changed", &PySIEffect::d_has_shape_changed)
        .def_readwrite("has_data_changed", &PySIEffect::d_data_changed)
        .def_readwrite("mouse_wheel_angle_px", &PySIEffect::mouse_wheel_angle_px)
        .def_readwrite("mouse_wheel_angle_degrees", &PySIEffect::mouse_wheel_angle_degrees)

        .enable_pickling()
        ;

    bp::enum_<int>("DataType")
        .value("INT", SI_DATA_TYPE_INT)
        .value("FLOAT", SI_DATA_TYPE_FLOAT)
        .value("STRING", SI_DATA_TYPE_STRING)

        .export_values()
        ;

    bp::enum_<int>("EffectType")
        .value("SI_CANVAS", SI_TYPE_CANVAS)
        .value("SI_CURSOR", SI_TYPE_CURSOR)
        .value("SI_MOUSE_CURSOR", SI_TYPE_MOUSE_CURSOR)
        .value("SI_CUSTOM", SI_TYPE_CUSTOM)

        .export_values()
        ;
}