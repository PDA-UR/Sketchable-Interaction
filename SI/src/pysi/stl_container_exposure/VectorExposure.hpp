

#ifndef SITEST_PYTHONICVECTORINDEXINGSUITE_HPP
#define SITEST_PYTHONICVECTORINDEXINGSUITE_HPP

#include <boost/python.hpp>
#include <vector>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

namespace bp = boost::python;

void IndexError() { PyErr_SetString(PyExc_IndexError, "Index out of range"); }

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

template <typename VectorType>
bp::class_<VectorType> create_vector(const char* name)
{
    return bp::class_<VectorType>(name)
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
