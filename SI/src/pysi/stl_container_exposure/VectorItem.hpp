

#ifndef SITEST_PYTHONICVECTORINDEXINGSUITE_HPP
#define SITEST_PYTHONICVECTORINDEXINGSUITE_HPP

#include <boost/python.hpp>
#include <vector>

#include <debug/Print.hpp>

namespace bp = boost::python;

void IndexError() { PyErr_SetString(PyExc_IndexError, "Index out of range"); }

template <typename T>
class VectorItem
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
};

#endif //SITEST_PYTHONICVECTORINDEXINGSUITE_HPP
