

#ifndef SITEST_PYTHONICVECTORINDEXINGSUITE_HPP
#define SITEST_PYTHONICVECTORINDEXINGSUITE_HPP

#include <boost/python.hpp>
#include <vector>
#include <debug/Print.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/core/demangle.hpp>
#include <glm/glm.hpp>

namespace bp = boost::python;

void IndexError()
{
    PyErr_SetString(PyExc_IndexError, "Index out of range");
}

template <typename T>
class VectorExposure
{
public:
    typedef typename T::value_type V;

    static V& get(T& x, int i)
    {
        if(i < 0)
            i += x.size();

        if(i >= 0 && i < x.size())
            return x[i];

        IndexError();
    }

    static void set(T& x, int i, V const& v)
    {
        if(i < 0)
            i += x.size();

        if(i >= 0 && i<x.size())
            x[i] = v;
        else
            IndexError();
    }

    static void del(T& x, int i)
    {
        if(i < 0)
            i += x.size();

        if(i >= 0 && i < x.size())
            x.erase(x.begin() + i);
        else IndexError();
    }

    static void add(T& x, V const& v)
    {
        x.push_back(v);
    }

    static bool in(T const& x, V const& v)
    {
        return std::find(x.begin(), x.end(), v) != x.end();
    }

    static int index(T const& x, V const& v)
    {
        int i = 0;

        for(typename T::const_iterator it = x.begin(); it!=x.end(); ++it, ++i)
            if(*it == v)
                return i;

        return -1;
    }
};

class VectorExposureVec3
{
public:
        static boost::shared_ptr<std::vector<glm::vec3>> init(const bp::list& list=bp::list())
        {
            auto self = boost::make_shared<std::vector<glm::vec3>>();

            Print::print("vec3");

            return self;
        }
};

class VectorExposureString
{
public:
    static boost::shared_ptr<std::vector<std::string>> init(const bp::list& list=bp::list())
    {
        auto self = boost::make_shared<std::vector<std::string>>();

        Print::print("string");

        return self;
    }
};

class VectorExposureLinkRelation
{
public:
    static boost::shared_ptr<std::vector<LinkRelation>> init(const bp::list& list=bp::list())
    {
        auto self = boost::make_shared<std::vector<LinkRelation>>();

        Print::print("lr");

        return self;
    }
};

template <typename VectorExposureType, typename VectorType>
bp::class_<VectorType> create_vector(const char* name)
{
    return bp::class_<VectorType>(name, bp::no_init)
            .def("__init__", bp::make_constructor(&VectorExposureType::init))
            .def("__len__", &VectorType::size)
            .def("clear", &VectorType::clear)
            .def("append", &VectorExposure<VectorType>::add)
            .def("__getitem__", &VectorExposure<VectorType>::get, bp::return_value_policy<bp::reference_existing_object>())
            .def("__setitem__", &VectorExposure<VectorType>::set)
            .def("__delitem__", &VectorExposure<VectorType>::del)
            .def("__iter__", bp::iterator<VectorType>())
            .def("__contains__", &VectorExposure<VectorType>::in)
            .def("index", &VectorExposure<VectorType>::index)
            ;
}

#endif //SITEST_PYTHONICVECTORINDEXINGSUITE_HPP
