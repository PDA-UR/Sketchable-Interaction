
#include "SuperEffect.hpp"

#include <utility>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>

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

int PySIEffect::on_enter(bp::object &other)
{
    return 0;
}

int PySIEffect::on_continuous(bp::object &other)
{
    return 0;
}

int PySIEffect::on_leave(bp::object &other)
{
    return 0;
}

namespace bp = boost::python;

BOOST_PYTHON_MODULE(libPySI)
{
    IterableConverter()
        .from_python<std::vector<int>>()
        .from_python<std::vector<std::vector<int>>>()
        .from_python<std::vector<std::vector<std::vector<int>>>>()
        .from_python<std::map<std::string, int>>()
        ;

    bp::class_<std::vector<std::vector<std::vector<int>>>>("int_vec_vec_vec")
            .def(bp::vector_indexing_suite<std::vector<std::vector<std::vector<int>>>>());

    bp::class_<std::vector<std::vector<int>>>("int_vec_vec")
            .def(bp::vector_indexing_suite<std::vector<std::vector<int>>>());

    bp::class_<std::vector<int>>("int_vec")
            .def(bp::vector_indexing_suite<std::vector<int>>() );

    bp::class_<std::map<std::string, int>>("string_int_map")
            .def(bp::map_indexing_suite<std::map<std::string, int>>());

//    bp::class_<SI::Capability>("PySICapability")
//            .def("capabilities", &SI::Capability::capabilities)
//            .def("add_capability", &SI::Capability::add_capability)
//            .def("remove_capability", &SI::Capability::remove_capability)
//            ;

    bp::class_<PySIEffect, boost::noncopyable>("PySIEffect", bp::init<>())
        .def("on_enter", &PySIEffect::on_enter)
        .def("on_continuous", &PySIEffect::on_continuous)
        .def("on_leave", &PySIEffect::on_leave)
        ;
}