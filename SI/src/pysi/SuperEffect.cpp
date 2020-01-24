
#include "SuperEffect.hpp"

#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include <sigrun/context/Capability.hpp>

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

BOOST_PYTHON_MODULE(libPySI)
{
    IterableConverter()
        .from_python<std::vector<int>>()
        .from_python<std::vector<std::vector<int>>>()
        .from_python<std::vector<std::vector<std::vector<int>>>>()
        .from_python<std::map<std::string, int>>()
        .from_python<std::map<std::string, bp::object>>()
//        .from_python<std::map<std::string, std::map<std::string, bp::object>>>()
        ;

    bp::class_<std::vector<std::vector<std::vector<int>>>>("int_vec_vec_vec")
            .def(bp::vector_indexing_suite<std::vector<std::vector<std::vector<int>>>>());

    bp::class_<std::vector<std::vector<int>>>("int_vec_vec")
            .def(bp::vector_indexing_suite<std::vector<std::vector<int>>>());

    bp::class_<std::vector<int>>("int_vec")
            .def(bp::vector_indexing_suite<std::vector<int>>() );

    bp::class_<std::map<std::string, int>>("string_int_map")
            .def(bp::map_indexing_suite<std::map<std::string, int>>());

    bp::class_<Capability>("PySICapability")
        .add_static_property("__TEST1__", bp::make_getter(&Capability::__test1__))
        .add_static_property("__TEST2__", bp::make_getter(&Capability::__test2__))
        ;

    bp::scope the_scope = bp::class_<PySIEffect>("PySIEffect")
    ;

    bp::class_<PySIEffect>("PySIEffect", bp::init<>())
        .add_property("cap_emit", &PySIEffect::__collision_emit__, &PySIEffect::__set_collision_emit__)
        .add_property("cap_recv", &PySIEffect::__collision_recv__, &PySIEffect::__set_collision_recv__)
        .add_property("cap_link_emit", &PySIEffect::__link_emit__, &PySIEffect::__set_link_emit__)
        .add_property("cap_link_recv", &PySIEffect::__link_recv__, &PySIEffect::__set_link_recv__)
        .add_property("x", &PySIEffect::__x__, &PySIEffect::__set_x__)
        .add_property("y", &PySIEffect::__y__, &PySIEffect::__set_y__)
        .add_property("angle_degres", &PySIEffect::__angle_degrees__, &PySIEffect::__set_angle_degrees__)
        .add_property("angle_radians", &PySIEffect::__angle_radians__, &PySIEffect::__set_angle_radians__)
        .add_property("color", &PySIEffect::__color__, &PySIEffect::__set_color__)
        .add_property("scale", &PySIEffect::__scale__, &PySIEffect::__set_scale__)
        .add_property("name", &PySIEffect::__name__, &PySIEffect::__set__name__)
        .add_property("region_type", &PySIEffect::__effect_type__, &PySIEffect::__set_effect_type__)
        .add_property("source", &PySIEffect::__source__, &PySIEffect::__set__source__)
        .add_property("texture_path", &PySIEffect::__texture_path__, &PySIEffect::__set__texture_path__)
        ;

    bp::enum_<PySIEffect::EffectType>("EffectType")
        .value("SI_CANVAS", PySIEffect::SI_CANVAS)
        .value("SI_CURSOR", PySIEffect::SI_CURSOR)
        .value("SI_MOUSE_CURSOR", PySIEffect::SI_MOUSE_CURSOR)
        .value("SI_CUSTOM", PySIEffect::SI_CUSTOM)
        .export_values()
        ;
}