
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

namespace bp = boost::python;

class Print : public std::ostringstream
{
public:
    template<typename TupleType, typename FunctionType>
    static void for_each(TupleType &&, FunctionType, std::integral_constant<size_t, std::tuple_size<typename std::remove_reference<TupleType>::type>::value>)
    {}

    template<std::size_t I, typename TupleType, typename FunctionType, typename = typename std::enable_if<
            I != std::tuple_size<typename std::remove_reference<TupleType>::type>::value>::type>
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
    static std::string _print(const std::vector<std::vector<T>> &v)
    {
        return std::transform_reduce(std::execution::par, v.begin(), v.end(), std::string("["), [](const std::string& a, const std::string& b)
        {
            return a + ", " + b;
        }, [](const std::vector<T>& value)
        {
            return _print(value);
        }) + "]";
    }

    template<typename T>
    static std::string _print(const std::vector<T>& v)
    {
        return std::transform_reduce(std::execution::par, v.begin(), v.end(), std::string("["), [](const std::string& a, const std::string& b)
        {
            return a + ", " + b;
        }, [](const T& value)
        {
            return _print(value);
        }) + "]";
    }

    template<typename T1, typename T2>
    static std::string _print(const std::map<T1, T2> &map)
    {
        return std::transform_reduce(std::execution::par_unseq, map.begin(), map.end(), std::string("{"), [](const std::string& a, const std::string& b)
        {
            return a + ", " + b;
        }, [](const std::pair<T1, T2>& pair)
        {
            return _print(pair.first) + ": " + _print(pair.second);
        }) + "}";
    }

    static std::string _print(const QString& qs)
    {
        return qs.toStdString();
    }

    static std::string _print(const QVariant& qv)
    {
        if(std::string(qv.typeName()) == "int")
            return std::to_string(qv.value<int>());

        if(std::string(qv.typeName()) == "float")
            return std::to_string(qv.value<float>());

        if(std::string(qv.typeName()) == "QString")
            return _print(qv.value<QString>());
    }

    template <typename T>
    static T _print(T& p)
    {
        return p;
    }

    static std::string _print(int32_t p)
    {
        return std::to_string(p);
    }

    static std::string _print(int16_t p)
    {
        return std::to_string(p);
    }

    static std::string _print(int64_t p)
    {
        return std::to_string(p);
    }

    static std::string _print(uint32_t p)
    {
        return std::to_string(p);
    }

    static std::string _print(uint16_t p)
    {
        return std::to_string(p);
    }

    static std::string _print(uint64_t p)
    {
        return std::to_string(p);
    }

    static std::string _print(float p)
    {
        return std::to_string(p);
    }

    static std::string _print(double p)
    {
        return std::to_string(p);
    }

    static std::string _print(char p)
    {
        return std::to_string(p);
    }

    static std::string _print(int8_t p)
    {
        return std::to_string(p);
    }

    static std::string _print(uint8_t p)
    {
        return std::to_string(p);
    }

    static std::string _print(const glm::vec3& p)
    {
        return "(" + std::to_string(p.x) + ", " + std::to_string(p.y) + ")";
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