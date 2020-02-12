

#ifndef SITEST_MAPEXPOSURE_HPP
#define SITEST_MAPEXPOSURE_HPP

#include <boost/python.hpp>
#include <glm/glm.hpp>
#include <map>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>

namespace bp = boost::python;

void KeyError() { PyErr_SetString(PyExc_KeyError, "Key not found"); }

template<typename T>
class MapExposure
{
public:
    typedef typename T::key_type K;
    typedef typename T::mapped_type V;

    static V& get(T& x, K const& i)
    {
        if(x.find(i) != x.end())
            return x[i];

        KeyError();
    }

    static void set(T& x, K const& i, V const& v)
    {
        x[i] = v; // use map autocreation feature
    }

    static void del(T& x, K const& i)
    {
        if(x.find(i) != x.end())
            x.erase(i);
        else
            KeyError();
    }

    static bool in(T const& x, K const& i)
    {
        return x.find(i) != x.end();
    }

    static bp::list keys(T const& x)
    {
        bp::list t;

        for(typename T::const_iterator it = x.begin(); it != x.end(); ++it)
            t.append(it->first);

        return t;
    }

    static bp::list values(T const& x)
    {
        bp::list t;

        for(typename T::const_iterator it = x.begin(); it != x.end(); ++it)
            t.append(it->second);

        return t;
    }

    static bp::list items(T const& x)
    {
        bp::list t;

        for(typename T::const_iterator it = x.begin(); it != x.end(); ++it)
            t.append(bp::make_tuple(it->first, it->second));

        return t;
    }

    static int index(T const& x, K const& k)
    {
        int i = 0;

        for(typename T::const_iterator it = x.begin(); it != x.end(); ++it, ++i)
            if( it->first == k ) return i;

        return -1;
    }
};

template <typename MapType>
bp::class_<MapType> create_map(const char* name)
{
    return bp::class_<MapType>(name)
            .def("__len__", &MapType::size)
            .def("clear", &MapType::clear)
            .def("__getitem__", &MapExposure<MapType>::get, bp::return_value_policy<bp::reference_existing_object>())
            .def("__setitem__", &MapExposure<MapType>::set)
            .def("__delitem__", &MapExposure<MapType>::del)
            .def("__contains__", &MapExposure<MapType>::in)
            .def("has_key", &MapExposure<MapType>::in)
            .def("keys", &MapExposure<MapType>::keys)
            .def("values", &MapExposure<MapType>::values)
            .def("items", &MapExposure<MapType>::items)
            .def("index", &MapExposure<MapType>::index)
            ;
}

#endif //SITEST_MAPEXPOSURE_HPP
