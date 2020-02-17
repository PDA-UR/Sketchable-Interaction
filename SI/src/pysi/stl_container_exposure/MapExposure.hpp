

#ifndef SITEST_MAPEXPOSURE_HPP
#define SITEST_MAPEXPOSURE_HPP

#include <boost/python.hpp>
#include <map>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <glm/glm.hpp>

namespace bp = boost::python;

/**
 * \brief global helper function to display a key error
 */
void KeyError()
{
    PyErr_SetString(PyExc_KeyError, "Key not found");
}

/**
 * \class MapExposure
 * \brief MapExposure class providing the interface for exposing STL maps to the python3 bindings (PySI) in a pythonic way
 * @tparam T the STL map to be exposed
 */
template<typename T>
class MapExposure
{
public:

    /**
     * @tparam K the type of key the STL map T contains
     */
    typedef typename T::key_type K;

    /**
     * @tparam V the type of value the STL map T contains
     */
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

class MapExposurePartialContour
{
public:
    static boost::shared_ptr<std::map<std::string, std::vector<glm::vec3>>> init(const bp::dict& dict=bp::dict())
    {
        auto self = boost::make_shared<std::map<std::string, std::vector<glm::vec3>>>();

        const bp::list keys = bp::extract<bp::list>(dict.keys());
        for(int i = 0; i < bp::len(keys); ++i)
        {
            const std::string key = bp::extract<std::string>(keys[i]);
            (*self)[key] = bp::extract<std::vector<glm::vec3>>(dict[keys[i]]);
        }

        return self;
    }

    static void set(std::map<std::string, std::vector<glm::vec3>>& self, const std::string& key, const std::vector<glm::vec3>& points)
    {
        self[key] = points;
    }

    static std::string repr(std::map<std::string, std::vector<glm::vec3>>& self)
    {
        std::string ret = "{";

        for(auto& [key, value]: self)
        {
            ret += (key + "[");

            for(auto& p: value)
                ret += "(" + std::to_string(p.x) + ", " + std::to_string(p.y) + "), ";

            ret += "]";
        }

        ret += "}";

        return ret;
    }

private:
};

class MapExposureString2Function
{
public:
    static boost::shared_ptr<std::map<std::string, bp::object>> init(const bp::dict& dict=bp::dict())
    {
        auto self = boost::make_shared<std::map<std::string, bp::object>>();

        const bp::list keys = bp::extract<bp::list>(dict.keys());
        for(int i = 0; i < bp::len(keys); ++i)
        {
            const std::string key = bp::extract<std::string>(keys[i]);

            (*self)[key] = dict[keys[i]];
        }

        return self;
    }

    static void set(std::map<std::string, bp::object>& self, const std::string& key, const bp::object& function)
    {
        self[key] = function;
    }

    static std::string repr(std::map<std::string, bp::object>& self)
    {
        std::string ret = "{";

        for(auto& [key, value]: self)
        {
            ret += (key + ": " + "function, ");
        }

        ret += "}";

        return ret;
    }

private:
};

class MapExposureString2_String2FunctionMap_Map
{
public:
    static boost::shared_ptr<std::map<std::string, std::map<std::string, bp::object>>> init(const bp::dict& dict=bp::dict())
    {
        auto self = boost::make_shared<std::map<std::string, std::map<std::string, bp::object>>>();

        const bp::list outer_keys = bp::extract<bp::list>(dict.keys());
        for(int i = 0; i < bp::len(outer_keys); ++i)
        {
            const bp::dict& inner_dict = bp::extract<bp::dict>(dict[outer_keys[i]]);
            const bp::list& inner_keys = bp::extract<bp::list>(inner_dict.keys());

            const std::string outer_key = bp::extract<std::string>(outer_keys[i]);

            for(int k = 0; k < bp::len(inner_keys); ++k)
            {
                const std::string inner_key = bp::extract<std::string>(inner_keys[k]);

                (*self)[outer_key][inner_key] = inner_dict[inner_keys[k]];
            }
        }

        return self;
    }

    static void set(std::map<std::string, std::map<std::string, bp::object>>& self, const std::string& key, const bp::dict& dict)
    {
        const bp::list keys = bp::extract<bp::list>(dict.keys());

        for(int i = 0; i < bp::len(keys); ++i)
        {
            const std::string& k = bp::extract<std::string>(keys[i]);
            const bp::object& v = dict[keys[i]];

            self[key][k] = v;
        }
    }

    static std::string repr(std::map<std::string, std::map<std::string, bp::object>>& self)
    {
        std::string ret = "{";

        for(auto& [key, value]: self)
        {
            ret += key + ": {";

            for(auto& [key2, value2]: value)
                ret += key2 + ": " + "function},";
        }

        ret += "}";

        return ret;
    }

private:
};

template <typename MapExposureType, typename MapType>
bp::class_<MapType> create_map(const char* name)
{
    return bp::class_<MapType>(name, bp::no_init)
            .def("__init__", bp::make_constructor(&MapExposureType::init, bp::default_call_policies(), (bp::arg("dict")=bp::dict())))
            .def("__len__", &MapType::size)
            .def("clear", &MapType::clear)
            .def("__getitem__", &MapExposure<MapType>::get, bp::return_value_policy<bp::reference_existing_object>())
            .def("__setitem__", &MapExposure<MapType>::set)
            .def("__setitem__", &MapExposureType::set)
            .def("__delitem__", &MapExposure<MapType>::del)
            .def("__contains__", &MapExposure<MapType>::in)
            .def("has_key", &MapExposure<MapType>::in)
            .def("keys", &MapExposure<MapType>::keys)
            .def("values", &MapExposure<MapType>::values)
            .def("items", &MapExposure<MapType>::items)
            .def("index", &MapExposure<MapType>::index)
            .def("__repr__", &MapExposureType::repr)
            ;
}

#endif //SITEST_MAPEXPOSURE_HPP
