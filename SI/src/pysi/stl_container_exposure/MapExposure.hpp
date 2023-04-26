
#ifndef SITEST_MAPEXPOSURE_HPP
#define SITEST_MAPEXPOSURE_HPP

#include <boost/python.hpp>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include <execution>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <glm/glm.hpp>
#include <pysi/PySIEffect.hpp>

namespace bp = boost::python;

/**
 * @class MapExposure
 * @brief MapExposure class providing the interface for exposing STL maps to the python3 bindings (PySI) in a pythonic way
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

    inline  static V& get(T& x, K const& i)
    {
        return x[i];
    }

    inline static void set(T& x, K const& i, V const& v)
    {
        x[i] = v;
    }

    inline static void del(T& x, K const& i)
    {
        if(x.find(i) != x.end())
            x.erase(i);
    }

    constexpr static bool in(T const& x, K const& i)
    {
        return x.find(i) != x.end();
    }

    static bp::list keys(T const& x)
    {
        bp::list t;

        std::for_each(x.begin(), x.end(), [&](auto& pair)
        {
           t.append(pair.first);
        });

        return t;
    }

    static bp::list values(T const& x)
    {
        bp::list t;

        std::for_each(x.begin(), x.end(), [&](auto& pair)
        {
            t.append(pair.second);
        });

        return t;
    }

    static bp::list items(T const& x)
    {
        bp::list t;

        std::for_each(x.begin(), x.end(), [&](auto& pair)
        {
            t.append(bp::make_tuple(pair.first, pair.second));
        });

        return t;
    }

    constexpr static int32_t index(T const& x, K const& k)
    {
        int i = 0;
        for(typename T::const_iterator it = x.begin(); it != x.end(); ++it, ++i)
            if( it->first == k )
                return i;

        return -1;
    }
};

/**
 * @class MapExposurePartialContour
 * @brief MapExposurePartialContour class providing the interface for exposing a STL map to the python3 bindings (PySI) in a pythonic way which provides the functionality to add partial contours, i.e. regions which are in the process of being drawn, based on the uuid of the used cursor.
 */
class MapExposurePartialContour
{
public:
    /**
     * @brief the constructor of the MapExposurePartialContour
     *
     * @param[in] dict the python dictionary which contains cursor uuids as keys and partial contours as values.
     *
     * @return a boost::shared_ptr<std::unordered_map<std::string, std::vector<glm::vec3>>> which stores the data of the given dictionary dict
     */
    static boost::shared_ptr<std::unordered_map<std::string, std::vector<glm::vec3>>> init(const bp::dict& dict=bp::dict())
    {
        auto self = boost::make_shared<std::unordered_map<std::string, std::vector<glm::vec3>>>();

        const bp::list keys = bp::extract<bp::list>(dict.keys());
        for(int i = 0; i < bp::len(keys); ++i)
        {
            const std::string key = bp::extract<std::string>(keys[i]);
            (*self)[key] = bp::extract<std::vector<glm::vec3>>(dict[keys[i]]);
        }

        return self;
    }

    /**
     * @brief a member function which adds a new key value pair to the given MapExposurePartialContour self
     *
     * @param[in, out] self a map to which a new key/value pair is to be added
     * @param[in] key a std::string which contains the uuid of a cursor used for drawing a region
     * @param[in] points a std::vector<glm::vec3> which contains the points of the partial contour which is drawn
     */
    inline static void set(std::unordered_map<std::string, std::vector<glm::vec3>>& self, const std::string& key, const std::vector<glm::vec3>& points)
    {
        self[key] = points;
    }

    /**
     * @brief a member function which returns a string which contains the data of the map in readable way which can also be used from python
     *
     * @param[in] self the map to be represented by a string
     *
     * @return the string containing the representation of the map
     */
    static std::string repr(std::unordered_map<std::string, std::vector<glm::vec3>>& self)
    {
        return std::transform_reduce(self.begin(), self.end(), std::string("{"), [](const std::string& a, const std::string& b)
        {
            return a + ", " + b;
        }, [](const std::pair<std::string, std::vector<glm::vec3>>& pair)
        {
            return pair.first + ": [" + std::transform_reduce(std::execution::par, pair.second.begin(), pair.second.end(), std::string(""), [](const std::string& a, const std::string& b)
            {
                return a + ", " + b;
            }, [](const glm::vec3& p)
            {
                return "[" + std::to_string(p.x) + ", " + std::to_string(p.y) + "]";
            }) + "]";
        }) + "}";
    }
};

/**
 * @class MapExposureString2Function
 * @brief MapExposureString2Function class providing the interface for exposing a STL map to the python3 bindings (PySI) in a pythonic way which provides the functionality to add a region event capaility as key and the event function as value.
 */
class MapExposureString2Function
{
public:
    /**
     * @brief the constructor of the MapExposureString2Function
     *
     * @param[in] dict the python dictionary which contains capabilities as keys and event functions as values.
     *
     * @return a boost::shared_ptr<std::unordered_map<std::string, bp::object>> which stores the data of the given dictionary dict
     */
    static boost::shared_ptr<std::unordered_map<std::string, bp::object>> init(const bp::dict& dict=bp::dict())
    {
        auto self = boost::make_shared<std::unordered_map<std::string, bp::object>>();

        const bp::list keys = bp::extract<bp::list>(dict.keys());
        for(uint32_t i = 0; i < bp::len(keys); ++i)
        {
            const std::string key = bp::extract<std::string>(keys[i]);

            (*self)[key] = dict[keys[i]];
        }

        return self;
    }

    /**
     * @brief a member function which adds a new key value pair to the given MapExposureString2Function self
     *
     * @param[in, out] self a map to which a new key/value pair is to be added
     * @param[in] key a std::string which contains the capability of an event
     * @param[in] function a boost::python::object which contains the function of an event
     */
    inline static void set(std::unordered_map<std::string, bp::object>& self, const std::string& key, const bp::object& function)
    {
        self[key] = function;
    }

    /**
     * @brief a member function which returns a string which contains the data of the map in readable way which can also be used from python
     *
     * @param[in] self the map to be represented by a string
     *
     * @return the string containing the representation of the map
     */
    static std::string repr(std::unordered_map<std::string, bp::object>& self)
    {
        return std::transform_reduce(self.begin(), self.end(), std::string("{"), [&](const std::string& a, const std::string& b)
        {
            return a + ", " + b;
        }, [&](const std::pair<std::string, bp::object>& pair)
        {
            if(!pair.second.is_none())
            {
                const std::string& str = bp::extract<std::string>(pair.second.attr("__name__"));
                return pair.first + ": " + "function: self." + str + ", ";
            }
            else
                return pair.first + ": " + "function: None, ";
        }) + "}";
    }

private:
};

/**
 * @class MapExposureString2_String2FunctionMap_Map
 * @brief MapExposureString2_String2FunctionMap_Map class providing the interface for exposing a STL map to the python3 bindings (PySI) in a pythonic way which provides the functionality of a map which uses capabilities as keys to other maps which use event keywords, such as on_enter, on_continous, and on_leave for collision events, or another capbility for linking events as keys to the event function.
 */
class MapExposureString2_String2FunctionMap_Map
{
public:
    /**
     * @brief the constructor of the MapExposureString2_String2FunctionMap_Map
     *
     * @param[in] dict the python dictionary which contains capabilities as keys and and an inner map as value which contains the event keywords or capabilities as keys and the event function as values.
     *
     * @return a boost::shared_ptr<std::unordered_map<std::string, std::unordered_map<std::string, bp::object>>> which stores the data of the given dictionary dict
     */
    static boost::shared_ptr<std::unordered_map<std::string, std::unordered_map<std::string, bp::object>>> init(const bp::dict& dict=bp::dict())
    {
        auto self = boost::make_shared<std::unordered_map<std::string, std::unordered_map<std::string, bp::object>>>();

        const bp::list outer_keys = bp::extract<bp::list>(dict.keys());
        for(uint32_t i = 0; i < bp::len(outer_keys); ++i)
        {
            const bp::dict& inner_dict = bp::extract<bp::dict>(dict[outer_keys[i]]);
            const bp::list& inner_keys = bp::extract<bp::list>(inner_dict.keys());

            const std::string outer_key = bp::extract<std::string>(outer_keys[i]);

            for(uint32_t k = 0; k < bp::len(inner_keys); ++k)
            {
                const std::string inner_key = bp::extract<std::string>(inner_keys[k]);

                (*self)[outer_key][inner_key] = inner_dict[inner_keys[k]];
            }
        }

        return self;
    }

    /**
     * @brief a member function which adds a new key value pair to the given MapExposureString2Function self
     *
     * @param[in, out] self a map to which a new key/value pair is to be added
     * @param[in] key a std::string which contains the capability of an event
     * @param[in] dict a boost::python::dict which contains the key/value pairs of event keywords or capabilities and event functions.
     */
    inline static void set(std::unordered_map<std::string, std::unordered_map<std::string, bp::object>>& self, const std::string& key, const bp::dict& dict)
    {
        const bp::list keys = bp::extract<bp::list>(dict.keys());

        for(uint32_t i = 0; i < bp::len(keys); ++i)
        {
            const std::string& k = bp::extract<std::string>(keys[i]);
            const bp::object& v = dict[keys[i]];

            self[key][k] = v;
        }
    }

    /**
     * @brief a member function which returns a string which contains the data of the map self in readable way which can also be used from python
     *
     * @param[in] self the map to be represented by a string
     *
     * @return the string containing the representation of the map
     */
    static std::string repr(std::unordered_map<std::string, std::unordered_map<std::string, bp::object>>& self)
    {
        return std::transform_reduce(self.begin(), self.end(), std::string("{"), [&](const std::string& a, const std::string& b)
        {
            return a + ", " + b;
        }, [&](const std::pair<std::string, std::unordered_map<std::string, bp::object>>& outer_pair)
        {
            return outer_pair.first + ": " + std::transform_reduce(outer_pair.second.begin(), outer_pair.second.end(), std::string("{"), [&](const std::string& a2, const std::string& b2)
            {
                return a2 + ", " + b2;
            }, [&](const std::pair<std::string, bp::object>& inner_pair)
            {
                if(!(inner_pair.second.is_none()))
                {
                    const std::string str = bp::extract<std::string>(inner_pair.second.attr("__name__"));
                    return inner_pair.first + ": " + "function: self." + str + ", ";
                }
                else
                    return inner_pair.first + ": " + "function: None, ";
            }) + "}";
        }) + "}";
    }

private:
};

/**
 * @brief a function which returns a boost::python::class_ which is called from module definition in PySI to expose the required map types to plugins using the template functions of class MapExposure<T>
 *
 * @tparam MapExposureType the type of the required exposure class: MapExposurePartialContour, MapExposureString2Function, MapExposureString2_String2FunctionMap_Map
 * @tparam MapType the type of the of the required map: std::unordered_map<std::string, std::vector<glm::vec3>>, std::unordered_map<std::string, bp::object>, std::unordered_map<std::string, std::unordered_map<std::string, bp::object>>,
 * @tparam PickleSuite the suite used for pickling: PartialContourPickleSuite, String2FunctionMapPickleSuite, String2String2FunctionMapMapPickleSuite,
 *
 * @param[in] name the name of the class which is used in python: PartialContour, String2FunctionMap, String2String2FunctionMapMap
 *
 * @return a boost::python::class_ which provides the class for use in python when called in a module definition
 *
 * @see MapExposure
 * @see MapExposurePartialContour
 * @see MapExposureString2Function
 * @see MapExposureString2_String2FunctionMap_Map
 * @see PartialContourPickleSuite
 * @see String2FunctionMapPickleSuite
 * @see String2String2FunctionMapMapPickleSuite
 */
template <typename MapExposureType, typename MapType, typename PickleSuite>
bp::class_<MapType> create_map(const char* name)
{
    return bp::class_<MapType>(name, bp::no_init)
            .def_pickle(PickleSuite())

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
