
#ifndef SIGRUN_PRINT_HPP
#define SIGRUN_PRINT_HPP

#include <boost/python.hpp>
#include <mutex>
#include <tuple>
#include <iostream>
#include <map>
#include <algorithm>
#include <numeric>
#include <execution>
#include <sstream>
#include <QString>
#include <QVariant>
#include <glm/glm.hpp>
#include <pysi/PySIEffect.hpp>
#include <sigrun/SIConstants.hpp>
#include <sigrun/network/TangibleObjectMessage.hpp>
#include <sigrun/region/RegionTransform.hpp>

namespace bp = boost::python;

class Print : public std::ostringstream
{
public:
    template<typename TupleType, typename FunctionType>
    static void for_each(TupleType &&, FunctionType, std::integral_constant<size_t, std::tuple_size<typename std::remove_reference<TupleType>::type>::value>)
    {}

    template<std::size_t I, typename TupleType, typename FunctionType, typename = typename std::enable_if<I != std::tuple_size<typename std::remove_reference<TupleType>::type>::value>::type>
    static void for_each(TupleType &&t, FunctionType f, std::integral_constant<size_t, I>)
    {
        f(std::get<I>(t));
        for_each(std::forward<TupleType>(t), f, std::integral_constant<size_t, I + 1>());
    }

    template<typename TupleType, typename FunctionType>
    static void for_each(TupleType &&t, FunctionType f)
    {
        for_each(std::forward<TupleType>(t), f, std::integral_constant<size_t, 0>());
    }

    Print() = default;

    ~Print()
    {
        std::lock_guard<std::mutex> guard(_mutexPrint);
        std::cout << this->str();
    }

    template<typename T>
    inline static std::string _print(const std::vector<std::vector<T>> &v)
    {
        if(v.empty())
            return "[]";

        std::string ret = std::transform_reduce(std::execution::par, v.begin(), v.end(), std::string("["), [](const std::string& a, const std::string& b)
        {
            return a + ", " + b;
        }, [](const std::vector<T>& value)
        {
            return _print(value);
        }) + "]";

        return "[" + ret.substr(2);
    }

    template<typename T>
    inline static std::string _print(const std::vector<T>& v)
    {
        if(v.empty())
            return "[]";

        std::string ret = std::transform_reduce(std::execution::par, v.begin(), v.end(), std::string(""), [](const std::string& a, const std::string& b)
        {
            return a + ", " + b;
        }, [](const T& value)
        {
            return _print(value);
        }) + "]";

        return "[" + ret.substr(2);
    }

    template<typename T1, typename T2>
    inline static std::string _print(const std::map<T1, T2> &map)
    {
        std::string ret = std::transform_reduce(map.begin(), map.end(), std::string("{"), [](const std::string& a, const std::string& b)
        {
            return a + ", " + b;
        }, [](const std::pair<T1, T2>& pair)
        {
            return _print(pair.first) + ": " + _print(pair.second);
        }) + "}";

        return "{" + ret.substr(2);
    }

    inline static std::string _print(const QString& qs)
    {
        return qs.toStdString();
    }

    inline static std::string _print(const QVariant& qv)
    {
        if(std::string(qv.typeName()) == SI_DATA_TYPE_NAME_INTEGER)
            return std::to_string(qv.value<int>());

        if(std::string(qv.typeName()) == SI_DATA_TYPE_NAME_FLOAT)
            return std::to_string(qv.value<float>());

        if(std::string(qv.typeName()) == SI_DATA_TYPE_NAME_QString)
            return _print(qv.value<QString>());
    }

    template <typename T>
    inline static T _print(T& p)
    {
        return p;
    }

    inline static std::string _print(int p)
    {
        return std::to_string(p);
    }

    inline static std::string _print(int16_t p)
    {
        return std::to_string(p);
    }

    inline static std::string _print(int64_t p)
    {
        return std::to_string(p);
    }

    inline static std::string _print(uint32_t p)
    {
        return std::to_string(p);
    }

    inline static std::string _print(uint16_t p)
    {
        return std::to_string(p);
    }

    inline static std::string _print(uint64_t p)
    {
        return std::to_string(p);
    }

    inline static std::string _print(float p)
    {
        return std::to_string(p);
    }

    inline static std::string _print(double p)
    {
        return std::to_string(p);
    }

    inline static std::string _print(char p)
    {
        return std::to_string(p);
    }

    inline static std::string _print(int8_t p)
    {
        return std::to_string(p);
    }

    inline static std::string _print(uint8_t p)
    {
        return std::to_string(p);
    }

    inline static std::string _print(const glm::vec3& p)
    {
        return "(" + std::to_string(p.x) + ", " + std::to_string(p.y) + ")";
    }

    inline static std::string _print(const glm::vec2& p)
    {
        return "(" + std::to_string(p.x) + ", " + std::to_string(p.y) + ")";
    }

    inline static std::string _print(const glm::ivec4& p)
    {
        return "(" + std::to_string(p.x) + ", " + std::to_string(p.y) + ", " + std::to_string(p.z) + ", " + std::to_string(p.w) + ")";
    }

    inline static std::string _print(const TangibleObjectMessage* msg)
    {
        return "TangibleObjectMessage(" + _print(msg->id()) + ", " + _print(msg->shape()) + ", " + msg->plugin_identifier() + ", " + _print(msg->x()) + ", " + _print(msg->y()) + ", " + _print(msg->is_click()) + ", " + _print(msg->is_drag()) + ", " + _print(msg->is_touch()) + ", " + _print(msg->is_alive()) + ", " + _print(msg->tracker_dimensions()) + ", " + _print(msg->links());
    }

    inline static std::string _print(RegionTransform* tform)
    {
        const glm::mat3x3& transform = tform->transform();

        const glm::vec3& r1 = transform[0];
        const glm::vec3& r2 = transform[1];
        const glm::vec3& r3 = transform[2];

        std::string ret = "|" + _print(r1.x) + " " + _print(r1.y) + " " + _print(r1.z) + "|\n"
                          "|" + _print(r2.x) + " " + _print(r2.y) + " " + _print(r2.z) + "|\n"
                          "|" + _print(r3.x) + " " + _print(r3.y) + " " + _print(r3.z) + "|";

        return ret;
    }

    inline static std::string _print(const bp::dict& d)
    {
        return _print(bp::str(d));
    }

    inline static std::string _print(const bp::str str)
    {
        const std::string& s = bp::extract<std::string>(str);
        return s;
    }

    template<class... Args>
    static void print(Args &&... args)
    {
        uint32_t i = 0;
        auto arguments = std::make_tuple(std::forward<Args>(args)...);

        for_each(arguments, [&](const auto &x)
        {
            if(i > 0)
                Print{} << ", ";

            Print{} << _print(x);

            ++i;
        });

        Print{} << std::endl;
    }

private:
    static std::mutex _mutexPrint;
};

#endif //SITEST_SIRENPRINT_HPP