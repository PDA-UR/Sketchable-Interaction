

#ifndef PYSI_SUPEREFFECT_HPP
#define PYSI_SUPEREFFECT_HPP

#include <boost/python.hpp>

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

class SuperEffect
{
public:
    virtual int on_enter(bp::object& other) = 0;
    virtual int on_continuous(bp::object& other) = 0;
    virtual int on_leave(bp::object& other) = 0;
};

class PySIEffect: public SuperEffect, public bp::wrapper<SuperEffect>
{
public:
    int on_enter(bp::object& other) override;
    int on_continuous(bp::object& other) override;
    int on_leave(bp::object& other) override;
};


#endif //PYSI_SUPEREFFECT_HPP
