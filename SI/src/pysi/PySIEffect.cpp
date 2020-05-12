
#include "PySIEffect.hpp"
#include "pysi/stl_container_exposure/MapExposure.hpp"
#include "pysi/stl_container_exposure/VectorExposure.hpp"
#include <sigrun/context/Context.hpp>
#include <sigrun/region/RegionResampler.hpp>
#include <debug/Print.hpp>

namespace bp = boost::python;

PySIEffect::PySIEffect(const std::vector<glm::vec3>& contour, const std::string& uuid, const std::string& tex_path, const bp::dict& kwargs)
    : d_data_changed(false), d_uuid(uuid)
{
    d_regions_marked_for_registration.reserve(10);
    d_link_relations.reserve(100);
    d_contour.reserve(STEPCOUNT);
    d_aabb.reserve(4);

    RegionResampler::resample(d_contour, contour);

    int32_t x_min = std::numeric_limits<int32_t>::max();
    int32_t x_max = std::numeric_limits<int32_t>::min();
    int32_t y_min = std::numeric_limits<int32_t>::max();
    int32_t y_max = std::numeric_limits<int32_t>::min();

    std::for_each(std::execution::par_unseq, d_contour.begin(), d_contour.end(), [&](auto& v)
    {
        x_max = v.x > x_max ? v.x : x_max;
        y_max = v.y > y_max ? v.y : y_max;
        x_min = v.x < x_min ? v.x : x_min;
        y_min = v.y < y_min ? v.y : y_min;
    });

    glm::vec3 tlc(x_min, y_min, 1), blc(x_min, y_max, 1), brc(x_max, y_max, 1), trc(x_max, y_min, 1);

    d_aabb = std::vector<glm::vec3>
    {
        tlc, blc, brc, trc
    };
}

void PySIEffect::__signal_deletion__()
{
    d_flagged_for_deletion = true;
}

bool PySIEffect::is_flagged_for_deletion()
{
    return d_flagged_for_deletion;
}

void PySIEffect::__embed_file_standard_appliation_into_context__(const std::string& uuid, const std::string& path)
{
    Context::SIContext()->external_application_manager()->launch_standard_application(uuid, path);
}

void PySIEffect::__destroy_embedded_file_standard_appliation_in_context__(const std::string& uuid)
{
    Context::SIContext()->external_application_manager()->terminate_application(uuid);
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
        case SI_LEFT_MOUSE_BUTTON: // left mouse button
            return d_is_left_mouse_clicked;
        case SI_RIGHT_MOUSE_BUTTON: // right mouse button
            return d_is_right_mouse_clicked;
        case SI_MIDDLE_MOUSE_BUTTON: // middle mouse button
            return d_is_middle_mouse_clicked;
        default:
            return false;
    }
}

void PySIEffect::set_mouse_pressed_capability(uint32_t btn, bool active)
{
    switch (btn)
    {
        case SI_LEFT_MOUSE_BUTTON: // left mouse button
            d_is_left_mouse_clicked = active;
            break;
        case SI_RIGHT_MOUSE_BUTTON: // right mouse button
            d_is_right_mouse_clicked = active;
            break;
        case SI_MIDDLE_MOUSE_BUTTON: // middle mouse button
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

std::vector<LinkCandidate>& PySIEffect::link_relations()
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

std::vector<glm::vec3> PySIEffect::get_shape()
{
    return d_contour;
}

void PySIEffect::set_shape(const std::vector<glm::vec3>& shape)
{
    if(!shape.empty())
    {
        d_contour.clear();

        RegionResampler::resample(d_contour, shape);

        int32_t x_min = std::numeric_limits<int32_t>::max();
        int32_t x_max = std::numeric_limits<int32_t>::min();
        int32_t y_min = std::numeric_limits<int32_t>::max();
        int32_t y_max = std::numeric_limits<int32_t>::min();

        std::for_each(std::execution::par_unseq, d_contour.begin(), d_contour.end(), [&](auto& v)
        {
            x_max = v.x > x_max ? v.x : x_max;
            y_max = v.y > y_max ? v.y : y_max;
            x_min = v.x < x_min ? v.x : x_min;
            y_min = v.y < y_min ? v.y : y_min;
        });

        glm::vec3 tlc(x_min, y_min, 1), blc(x_min, y_max, 1), brc(x_max, y_max, 1), trc(x_max, y_min, 1);

        d_aabb = std::vector<glm::vec3>
        {
            tlc, blc, brc, trc
        };

        d_recompute_mask = true;
    }
}

void PySIEffect::__create_region__(const std::vector<glm::vec3>& contour, const std::string& name)
{
    Context::SIContext()->register_new_region_via_name(contour, name);
}

void PySIEffect::__create_region__(const bp::list& contour, const std::string& name)
{
    std::vector<glm::vec3> _contour;
    _contour.reserve(bp::len(contour));

    HANDLE_PYTHON_CALL(PY_ERROR, "Passed list is not trivially convertible to SIGRun's internal datastructures. Try [[x1, y1], [x2, y2], ..., [xn, yn]].",
        for(uint32_t i = 0; i < bp::len(contour); ++i)
        {
            float x = bp::extract<float>(contour[i][0]);
            float y = bp::extract<float>(contour[i][1]);

            _contour.emplace_back(x, y, 1);
        }
    )

    if(!_contour.empty())
        __create_region__(_contour, name);
}

std::vector<std::string> PySIEffect::__available_plugins_by_name__()
{
    if(Context::SIContext())
        return Context::SIContext()->available_plugins_names();

    return std::vector<std::string>();
}

bp::tuple PySIEffect::__context_dimensions__()
{
    return bp::make_tuple(Context::SIContext()->width(), Context::SIContext()->height());
}

BOOST_PYTHON_MODULE(libPySI)
{
    bp::scope the_scope = bp::class_<PySIEffect>("PySIEffect", bp::init<const std::vector<glm::vec3>&, const std::string&, const std::string&, const bp::dict&>())
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

    bp::class_<LinkCandidate>("LinkRelation", bp::init<const std::string&, const std::string&, const std::string&, const std::string&>())
        .def_readwrite("sender", &LinkCandidate::sender)
        .def_readwrite("sender_attrib", &LinkCandidate::sender_attrib)
        .def_readwrite("recv", &LinkCandidate::recv)
        .def_readwrite("recv_attrib", &LinkCandidate::recv_attrib)

        .enable_pickling()
        ;

    create_vector<VectorExposureVec3, std::vector<glm::vec3>>("PointVector");
    create_vector<VectorExposureLinkRelation, std::vector<LinkCandidate>>("LinkRelationVector");
    create_vector<VectorExposureString, std::vector<std::string>>("StringVector");
    create_map<MapExposurePartialContour, std::map<std::string, std::vector<glm::vec3>>>("PartialContour");
    create_map<MapExposureString2Function, std::map<std::string, bp::object>>("String2FunctionMap");
    create_map<MapExposureString2_String2FunctionMap_Map, std::map<std::string, std::map<std::string, bp::object>>>("String2_String2FunctionMap_Map");

    bp::scope().attr("ON_ENTER") = SI_COLLISION_EVENT_ON_ENTER;
    bp::scope().attr("ON_CONTINUOUS") = SI_COLLISION_EVENT_ON_CONTINUOUS;
    bp::scope().attr("ON_LEAVE") = SI_COLLISION_EVENT_ON_LEAVE;

    bp::scope().attr("MOVE") = SI_CAPABILITY_COLLISION_MOVE;
    bp::scope().attr("BTN") = SI_CAPABILITY_COLLISION_BUTTON;
    bp::scope().attr("OPEN_ENTRY") = SI_CAPABILITY_COLLISION_OPEN_ENTRY;
    bp::scope().attr("PARENT") = SI_CAPABILITY_COLLISION_PARENT;
    bp::scope().attr("SKETCH") = SI_CAPABILITY_COLLISION_SKETCH;
    bp::scope().attr("CLICK") = SI_CAPABILITY_COLLISION_CLICK;
    bp::scope().attr("DELETION") = SI_CAPABILITY_COLLISION_DELETION;
    bp::scope().attr("PREVIEW") = SI_CAPABILITY_COLLISION_PREVIEW;

    bp::scope().attr("POSITION") = SI_CAPABILITY_LINK_POSITION;
    bp::scope().attr("ROTATION") = SI_CAPABILITY_LINK_ROTATION;
    bp::scope().attr("SCALE") = SI_CAPABILITY_LINK_SCALE;
    bp::scope().attr("COLOR") = SI_CAPABILITY_LINK_COLOR;
    bp::scope().attr("GEOMETRY") = SI_CAPABILITY_LINK_GEOMETRY;

    bp::scope().attr("SI_STD_NAME_DIRECTORY") = SI_NAME_EFFECT_DIRECTORY;
    bp::scope().attr("SI_STD_NAME_TEXTFILE") = SI_NAME_EFFECT_TEXTFILE;
    bp::scope().attr("SI_STD_NAME_IMAGEFILE") = SI_NAME_EFFECT_IMAGEFILE;
    bp::scope().attr("SI_STD_NAME_UNKNOWNFILE") = SI_NAME_EFFECT_UNKNOWNFILE;
    bp::scope().attr("SI_STD_NAME_BUTTON") = SI_NAME_EFFECT_BUTTON;
    bp::scope().attr("SI_STD_NAME_TAG") = SI_NAME_EFFECT_TAG;
    bp::scope().attr("SI_STD_NAME_DELETION") = SI_NAME_EFFECT_DELETION;
    bp::scope().attr("SI_STD_NAME_OPEN_ENTRY") = SI_NAME_EFFECT_OPEN_ENTRY;
    bp::scope().attr("SI_STD_NAME_MOUSE_CURSOR") = SI_NAME_EFFECT_MOUSE_CURSOR;
    bp::scope().attr("SI_STD_NAME_CANVAS") = SI_NAME_EFFECT_CANVAS;
    bp::scope().attr("SI_STD_NAME_SIMPLE_NOTIFICATION") = SI_NAME_EFFECT_SIMPLE_NOTIFICATION;
    bp::scope().attr("SI_STD_NAME_ENTRY") = SI_NAME_EFFECT_ENTRY;
    bp::scope().attr("SI_STD_NAME_CONTAINER") = SI_NAME_EFFECT_CONTAINER;
    bp::scope().attr("SI_STD_NAME_PLACEHOLDER") = SI_NAME_EFFECT_PLACEHOLDER;
    bp::scope().attr("SI_STD_NAME_PREVIEW") = SI_NAME_EFFECT_PREVIEW;
    bp::scope().attr("SI_STD_NAME_PALETTE") = SI_NAME_EFFECT_PALETTE;

    bp::class_<PySIEffect, boost::noncopyable>("PySIEffect", bp::init<const std::vector<glm::vec3>&, const std::string&, const std::string&, const bp::dict&>())
        .def("__add_data__", &PySIEffect::__add_data__)
        .def("__signal_deletion__", &PySIEffect::__signal_deletion__)
        .def("__show_folder_contents_page__", &PySIEffect::__show_folder_contents__)
        .def("__embed_file_standard_appliation_into_context__", &PySIEffect::__embed_file_standard_appliation_into_context__)
        .def("__destroy_embedded_window__", &PySIEffect::__destroy_embedded_file_standard_appliation_in_context__)
        .def<void (PySIEffect::*)(const std::vector<glm::vec3>&, const std::string&)>("__create_region__", &PySIEffect::__create_region__)
        .def<void (PySIEffect::*)(const bp::list&, const std::string&)>("__create_region__", &PySIEffect::__create_region__)
        .def("__available_plugins_by_name__", &PySIEffect::__available_plugins_by_name__)
        .def("__context_dimensions__", &PySIEffect::__context_dimensions__)

        .add_property("shape", &PySIEffect::get_shape, &PySIEffect::set_shape)

        .def_readonly("aabb", &PySIEffect::d_aabb)
        .def_readwrite("__recompute_collision_mask__", &PySIEffect::d_recompute_mask)

        .def_readwrite("__partial_regions__", &PySIEffect::d_partial_regions)
        .def_readwrite("__registered_regions__", &PySIEffect::d_regions_marked_for_registration)
        .def_readwrite("cap_emit", &PySIEffect::d_cap_collision_emit)
        .def_readwrite("cap_recv", &PySIEffect::d_cap_collision_recv)
        .def_readwrite("cap_link_emit", &PySIEffect::d_cap_link_emit)
        .def_readwrite("cap_link_recv", &PySIEffect::d_cap_link_recv)
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
        .value("SI_ENTRY", SI_TYPE_ENTRY)
        .value("SI_PREVIEW", SI_TYPE_PREVIEW)
        .value("SI_PALETTE", SI_TYPE_PALETTE)

        .export_values()
        ;
}