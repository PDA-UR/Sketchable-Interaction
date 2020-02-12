

#ifndef SITEST_MAPITEM_HPP
#define SITEST_MAPITEM_HPP

#include <boost/python.hpp>
#include <glm/glm.hpp>
#include <map>

namespace bp = boost::python;

void KeyError() { PyErr_SetString(PyExc_KeyError, "Key not found"); }

template<typename T>
class MapItem
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

        for(typename T::const_iterator it=x.begin(); it!=x.end(); ++it)
            t.append(it->first);

        return t;
    }

    static bp::list values(T const& x)
    {
        bp::list t;

        for(typename T::const_iterator it=x.begin(); it!=x.end(); ++it)
            t.append(it->second);

        return t;
    }

    static bp::list items(T const& x)
    {
        bp::list t;

        for(typename T::const_iterator it=x.begin(); it!=x.end(); ++it)
            t.append(bp::make_tuple(it->first, it->second));

        return t;
    }
};

#endif //SITEST_MAPITEM_HPP
