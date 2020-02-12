
#include "SuperEffect.hpp"

#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include <sigrun/context/Capability.hpp>
#include "pysi/stl_container_exposure/MapExposure.hpp"
#include "pysi/stl_container_exposure/VectorExposure.hpp"

template<typename Container>
IterableConverter& IterableConverter::from_python()
{
    bp::converter::registry::push_back(&IterableConverter::convertible, &IterableConverter::construct<Container>, bp::type_id<Container>());

    // Support chaining.
    return *this;
}

void* IterableConverter::convertible(PyObject* object)
{
    return PyObject_GetIter(object) ? object : nullptr;
}

template<typename Container>
void IterableConverter::construct(PyObject *object, bp::converter::rvalue_from_python_stage1_data *data)
{
    // Object is a borrowed reference, so create a handle indicting it is
    // borrowed for proper reference counting.
    bp::handle<> handle(bp::borrowed(object));

    // Obtain a handle to the memory block that the converter has allocated
    // for the C++ type.
    typedef bp::converter::rvalue_from_python_storage<Container> storage_type;

    void* storage = reinterpret_cast<storage_type *>(data)->storage.bytes;

    typedef bp::stl_input_iterator<typename Container::value_type> iterator;

    // Allocate the C++ type into the converter's memory block, and assign
    // its handle to the converter's convertible variable.  The C++
    // container is populated by passing the begin and end iterators of
    // the python object to the container's constructor.
    new(storage) Container( iterator(bp::object(handle)), iterator());
    data->convertible = storage;
}

namespace bp = boost::python;

void PySIEffect::__set_angle_degrees__(float angle)
{
    d_angle_deg = angle;
    d_angle_radians = TO_RADIANS(angle);
}

void PySIEffect::__set_angle_radians__(float angle)
{
    d_angle_radians = angle;
    d_angle_deg = TO_DEGREES(angle);
}

float PySIEffect::__angle_radians__()
{
    return d_angle_radians;
}

float PySIEffect::__angle_degrees__()
{
    return d_angle_deg;
}

void PySIEffect::__set_x__(float x)
{
    d_x = (int) std::round(x);
}

void PySIEffect::__set_y__(float y)
{
    d_y = (int) std::round(y);
}

int PySIEffect::__x__()
{
    return d_x;
}

int PySIEffect::__y__()
{
    return d_y;
}

int PySIEffect::x()
{
    return __x__();
}

int PySIEffect::y()
{
    return __y__();
}

void PySIEffect::__set_color__(const std::vector<int>& rgba)
{
    d_color = glm::vec4(rgba[0], rgba[1], rgba[2], rgba[3]);
}

const std::vector<int> PySIEffect::__color__() const
{
    return std::vector<int> {(int) d_color.r, (int) d_color.g, (int) d_color.b, (int) d_color.a};
}

void PySIEffect::__set_scale__(float factor)
{
    d_scale = factor;
}

const float PySIEffect::__scale__() const
{
    return d_scale;
}

void PySIEffect::__set_link_emit__(const bp::dict& dict)
{
    d_cap_link_emit.clear();

    const bp::list& keys = dict.keys();

    for(int i = 0; i < bp::len(keys); ++i)
        d_cap_link_emit.insert({bp::extract<std::string>(keys[i]), dict[keys[i]]});
}

const bp::dict PySIEffect::__link_emit__() const
{
    bp::dict ret;

    for(auto& [key, value]: d_cap_link_emit)
        ret[key] = *value;

    return ret;
}

void PySIEffect::__set_link_recv__(const bp::dict& dict)
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

const bp::dict PySIEffect::__link_recv__() const
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

void PySIEffect::__set__name__(const std::string& name)
{
    d_name = name;
}

void PySIEffect::__set_qml_path__(const std::string& path)
{
    d_qml_path = path;
}

void PySIEffect::__set_source__(const std::string& source)
{
    d_source = source;
}

void PySIEffect::__set_effect_type__(int type)
{
    d_effect_type = type;
}

const std::string PySIEffect::__name__() const
{
    return d_name;
}

const std::string PySIEffect::__qml_path__() const
{
    return d_qml_path;
}

const std::string PySIEffect::__source__() const
{
    return d_source;
}

const int PySIEffect::__effect_type__() const
{
    return d_effect_type;
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

bool PySIEffect::__is_left_mouse_clicked()
{
    return d_is_left_mouse_clicked;
}

void PySIEffect::__set_left_mouse_clicked__(bool active)
{
    d_is_left_mouse_clicked = active;
}

bool PySIEffect::__is_right_mouse_clicked()
{
    return d_is_right_mouse_clicked;
}

void PySIEffect::__set_right_mouse_clicked__(bool active)
{
    d_is_right_mouse_clicked = active;
}

bool PySIEffect::__is_middle_mouse_clicked()
{
    return d_is_middle_mouse_clicked;
}

void PySIEffect::__set_middle_mouse_clicked__(bool active)
{
    d_is_middle_mouse_clicked = active;
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

void PySIEffect::__register_region__(const std::string& sender_id)
{
    d_regions_marked_for_registration.push_back(sender_id);
}

std::vector<std::string>& PySIEffect::regions_for_registration()
{
    return d_regions_marked_for_registration;
}

void PySIEffect::__set_uuid__(const std::string& uuid)
{
    d_uuid = uuid;
}

const std::string PySIEffect::__uuid__() const
{
    return d_uuid;
}

void PySIEffect::__set_regions_for_registration__(const std::vector<std::string>& candidates)
{
    d_regions_marked_for_registration = candidates;
}

std::vector<std::string> PySIEffect::__regions_for_registration__()
{
    return d_regions_marked_for_registration;
}

std::map<std::string, std::vector<glm::vec3>>& PySIEffect::partial_region_contours()
{
    return d_partial_regions;
}

void PySIEffect::__register_link__(const std::string& sender, const std::string& sender_attrib, const std::string& recv, const std::string& recv_attrib)
{
    d_link_relations.push_back(std::make_shared<LinkRelation>(sender, sender_attrib, recv, recv_attrib));
}

void PySIEffect::__remove_link__(const std::string& sender, const std::string& sender_attrib, const std::string& recv, const std::string& recv_attrib)
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

void PySIEffect::__set_width__(float width)
{
    d_width = width;
}

void PySIEffect::__set_height__(float height)
{
    d_height = height;
}

int PySIEffect::__width__()
{
    return d_width;
}

int PySIEffect::__height__()
{
    return d_height;
}

int PySIEffect::width()
{
    return d_width;
}

int PySIEffect::height()
{
    return d_height;
}

std::vector<std::shared_ptr<LinkRelation>>& PySIEffect::link_relations()
{
    return d_link_relations;
}

void PySIEffect::__set_data__(const std::string &key, const bp::object &value, const int type)
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

bool PySIEffect::has_data_changed()
{
    return d_data_changed;
}

////////////////////////////////////////////////////////////////////////

// Found at https://stackoverflow.com/questions/27077518/wrapping-an-stdvector-using-boostpython-vector-indexing-suite
// and at https://wiki.python.org/moin/boost.python/StlContainers

/// @brief Mockup type.


BOOST_PYTHON_MODULE(libPySI)
{
    IterableConverter()
        .from_python<std::vector<int>>()
        .from_python<std::vector<std::string>>()
//        .from_python<std::map<std::string, bp::object>>()
//        .from_python<std::map<std::string, std::map<std::string, bp::object>>>()
        ;

    bp::class_<std::vector<int>>("int_vec")
            .def(bp::vector_indexing_suite<std::vector<int>>() );

    bp::class_<std::vector<std::string>>("string_vec")
            .def(bp::vector_indexing_suite<std::vector<std::string>>() );

//    bp::class_<std::map<std::string, bp::object>>("string_bpo_map")
//            .def(bp::map_indexing_suite<std::map<std::string, bp::object>>());

//    bp::class_<std::map<std::string, std::map<std::string, bp::object>>>("string_map_string_bpo_map")
//            .def(bp::map_indexing_suite<std::map<std::string, std::map<std::string, bp::object>>>());

    bp::class_<Capability>("PySICapability")
        .add_static_property("__TEST1__", bp::make_getter(&Capability::__test1__))
        .add_static_property("__TEST2__", bp::make_getter(&Capability::__test2__))
        ;

    bp::scope the_scope = bp::class_<PySIEffect>("PySIEffect")
    ;

    bp::class_<glm::vec2>("Point2", bp::init<float, float>())
            .def_readwrite("x", &glm::vec2::x)
            .def_readwrite("y", &glm::vec2::y)
            ;

    bp::class_<glm::vec3>("Point3", bp::init<float, float, float>())
            .def_readwrite("x", &glm::vec3::x)
            .def_readwrite("y", &glm::vec3::y)
            .def_readwrite("z", &glm::vec3::z)
            ;

    create_vector<std::vector<glm::vec3>>("PointVector");
    create_map<std::map<std::string, std::vector<glm::vec3>>>("PartialContour");
    create_map<std::map<std::string, bp::object>>("String2FunctionMap");
    create_map<std::map<std::string, std::map<std::string, bp::object>>>("CollisionEventMap");

    bp::class_<PySIEffect, boost::noncopyable>("PySIEffect", bp::init<>())
        .def("register_region", &PySIEffect::__register_region__)
        .def("register_link", &PySIEffect::__register_link__)
        .def("remove_link", &PySIEffect::__remove_link__)
        .def("set_data", &PySIEffect::__set_data__)

        .def_readwrite("__partial_regions__", &PySIEffect::d_partial_regions)
        .def_readwrite("cap_emit", &PySIEffect::d_cap_collision_emit)
        .def_readwrite("cap_recv", &PySIEffect::d_cap_collision_recv)


        .add_property("__regions_for_registration__", &PySIEffect::__regions_for_registration__, &PySIEffect::__set_regions_for_registration__)
        .add_property("left_mouse_clicked", &PySIEffect::__is_left_mouse_clicked, &PySIEffect::__set_left_mouse_clicked__)
        .add_property("right_mouse_clicked", &PySIEffect::__is_right_mouse_clicked, &PySIEffect::__set_right_mouse_clicked__)
        .add_property("middle_mouse_clicked", &PySIEffect::__is_middle_mouse_clicked, &PySIEffect::__set_middle_mouse_clicked__)
//        .add_property("cap_emit", &PySIEffect::__collision_emit__, &PySIEffect::__set_collision_emit__)
//        .add_property("cap_recv", &PySIEffect::__collision_recv__, &PySIEffect::__set_collision_recv__)
        .add_property("cap_link_emit", &PySIEffect::__link_emit__, &PySIEffect::__set_link_emit__)
        .add_property("cap_link_recv", &PySIEffect::__link_recv__, &PySIEffect::__set_link_recv__)

        .add_property("x", &PySIEffect::__x__, &PySIEffect::__set_x__)
        .add_property("y", &PySIEffect::__y__, &PySIEffect::__set_y__)
        .add_property("width", &PySIEffect::__width__, &PySIEffect::__set_width__)
        .add_property("height", &PySIEffect::__height__, &PySIEffect::__set_height__)
        .add_property("angle_degres", &PySIEffect::__angle_degrees__, &PySIEffect::__set_angle_degrees__)
        .add_property("angle_radians", &PySIEffect::__angle_radians__, &PySIEffect::__set_angle_radians__)
        .add_property("color", &PySIEffect::__color__, &PySIEffect::__set_color__)
        .add_property("scale", &PySIEffect::__scale__, &PySIEffect::__set_scale__)
        .add_property("name", &PySIEffect::__name__, &PySIEffect::__set__name__)
        .add_property("_uuid", &PySIEffect::__uuid__, &PySIEffect::__set_uuid__)
        .add_property("region_type", &PySIEffect::__effect_type__, &PySIEffect::__set_effect_type__)
        .add_property("source", &PySIEffect::__source__, &PySIEffect::__set_source__)
        .add_property("qml_path", &PySIEffect::__qml_path__, &PySIEffect::__set_qml_path__)

        .enable_pickling()
        ;

    bp::enum_<int>("DataType")
        .value("INT", SI_DATA_TYPE_INT)
        .value("FLOAT", SI_DATA_TYPE_FLOAT)
        .value("STRING", SI_DATA_TYPE_STRING)
        ;

    bp::enum_<int>("EffectType")
        .value("SI_CANVAS", SI_TYPE_CANVAS)
        .value("SI_CURSOR", SI_TYPE_CURSOR)
        .value("SI_MOUSE_CURSOR", SI_TYPE_MOUSE_CURSOR)
        .value("SI_CUSTOM", SI_TYPE_CUSTOM)
        .export_values()
        ;
}