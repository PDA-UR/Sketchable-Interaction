
#include "PySIEffect.hpp"
#include <sigrun/context/Capability.hpp>
#include "pysi/stl_container_exposure/MapExposure.hpp"
#include "pysi/stl_container_exposure/VectorExposure.hpp"
#include <sigrun/context/Context.hpp>

namespace bp = boost::python;

void PySIEffect::init(const std::vector<glm::vec3>& contour, const std::vector<glm::vec3>& aabb, const std::string& uuid, const bp::dict& kwargs)
{
    d_contour = contour;
    d_aabb = aabb;
    d_uuid = uuid;
    d_has_shape_changed = false;
    d_require_resample = false;

    d_regions_marked_for_registration.reserve(10);
    d_link_relations.reserve(100);
    d_contour.reserve(64);
    d_aabb.reserve(4);
}

void PySIEffect::signal_deletion()
{
    d_flagged_for_deletion = true;
}

bool PySIEffect::is_flagged_for_deletion()
{
    return d_flagged_for_deletion;
}


// has to be set to false from elsewhere (happens in Region.cpp, where value is used
void PySIEffect::notify_shape_changed(bool resample)
{
    d_has_shape_changed = true;
    d_require_resample = resample;
}

void PySIEffect::__embed_file_standard_appliation_into_context__(const std::string& uuid, const std::string& path)
{
    Context::SIContext()->launch_external_application_with_file(uuid, path);
}

void PySIEffect::__destroy_embedded_file_standard_appliation_in_context__(const std::string& uuid)
{
    Context::SIContext()->terminate_external_application_with_file(uuid);
}

uint32_t PySIEffect::has_shape_changed()
{
    uint32_t ret = 0;

    if(d_has_shape_changed)
        ret |= REQUIRES_NEW_SHAPE;

    if(d_require_resample)
        ret |= REQUIRES_RESAMPLE;

    return ret;
}

const uint32_t PySIEffect::x() const
{
    return d_x;
}

const uint32_t PySIEffect::y() const
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

const uint32_t PySIEffect::effect_type() const
{
    return d_effect_type;
}

bool PySIEffect::has_mouse_pressed_capability(uint32_t btn)
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

void PySIEffect::set_mouse_pressed_capability(uint32_t btn, bool active)
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

const uint32_t PySIEffect::width() const
{
    return d_width;
}

const uint32_t PySIEffect::height() const
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

void PySIEffect::__add_data__(const std::string &key, const bp::object &value, const uint32_t type)
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

        case SI_DATA_TYPE_BOOL:
            d_data[QString(key.c_str())] = QVariant(bp::extract<bool>(value));
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

void PySIEffect::__show_folder_contents__(const std::vector<std::string>& page_contents, const std::string& uuid, const bool with_btns)
{
    Context::SIContext()->spawn_folder_contents_as_regions(page_contents, uuid, with_btns);
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
        .def("__init__", bp::make_constructor(&PySIEffect::init, bp::default_call_policies(), (bp::arg("shape")=std::vector<glm::vec3>(), bp::arg("aabb")=std::vector<glm::vec3>(), bp::arg("uuid")=std::string(), bp::arg("kwargs")=bp::dict())))
        .def("add_data", &PySIEffect::__add_data__)
        .def("notify_shape_changed", &PySIEffect::notify_shape_changed)
        .def("signal_deletion", &PySIEffect::signal_deletion)
        .def("show_folder_contents_page", &PySIEffect::__show_folder_contents__)
        .def("embed_file_standard_appliation_into_context", &PySIEffect::__embed_file_standard_appliation_into_context__)
        .def("destroy_embedded_window", &PySIEffect::__destroy_embedded_file_standard_appliation_in_context__)

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
        .def_readwrite("require_resample", &PySIEffect::d_require_resample)
        .def_readwrite("has_data_changed", &PySIEffect::d_data_changed)
        .def_readwrite("mouse_wheel_angle_px", &PySIEffect::mouse_wheel_angle_px)
        .def_readwrite("mouse_wheel_angle_degrees", &PySIEffect::mouse_wheel_angle_degrees)

        .enable_pickling()
        ;

    bp::enum_<uint32_t>("DataType")
        .value("INT", SI_DATA_TYPE_INT)
        .value("FLOAT", SI_DATA_TYPE_FLOAT)
        .value("BOOL", SI_DATA_TYPE_BOOL)
        .value("STRING", SI_DATA_TYPE_STRING)

        .export_values()
        ;

    bp::enum_<uint32_t>("EffectType")
        .value("SI_CANVAS", SI_TYPE_CANVAS)
        .value("SI_CURSOR", SI_TYPE_CURSOR)
        .value("SI_MOUSE_CURSOR", SI_TYPE_MOUSE_CURSOR)
        .value("SI_EXTERNAL_APPLICATION_CONTAINER", SI_TYPE_EXTERNAL_APPLICATION_CONTAINER)
        .value("SI_DIRECTORY", SI_TYPE_DIRECTORY)
        .value("SI_TEXT_FILE", SI_TYPE_TEXT_FILE)
        .value("SI_IMAGE_FILE", SI_TYPE_IMAGE_FILE)
        .value("SI_UNKNOWN_FILE", SI_TYPE_UNKNOWN_FILE)
        .value("SI_DELETION", SI_TYPE_DELETION)
        .value("SI_BUTTON", SI_TYPE_BUTTON)
        .value("SI_NOTIFICATION", SI_TYPE_NOTIFICATION)
        .value("SI_CUSTOM", SI_TYPE_CUSTOM)

        .export_values()
        ;
}