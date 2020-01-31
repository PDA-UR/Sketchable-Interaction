
#ifndef SIGRUN_PRINT_HPP
#define SIGRUN_PRINT_HPP

#include <boost/python.hpp>
#include <mutex>
#include <tuple>
#include <iostream>
#include <map>
#include <sstream>

namespace bp = boost::python;

class Print : public std::ostringstream
{
public:
    template<typename TupleType, typename FunctionType>
    static void for_each(TupleType &&, FunctionType,
                         std::integral_constant<size_t, std::tuple_size<typename std::remove_reference<TupleType>::type>::value>)
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
        std::string os;

        os = "[";

        for (int i = 0; i < v.size(); ++i)
        {
            os += "[";
            for (int k = 0; k < v[i].size(); k++)
            {
                os += v[i][k];

                if (k != v.size() - 1)
                    os += ", ";
            }
            os += "]";
        }

        os += "]";

        return os;
    }

    template<typename T>
    static std::string _print(const std::vector<T>& v)
    {
        std::string os;

        os += "[";

        for (int i = 0; i < v.size(); ++i)
        {
            os += v[i];

            if (i != v.size() - 1)
                os += ", ";
        }

        os += "]";

        return os;
    }

    template<typename T1, typename T2>
    static std::string _print(const std::map<T1, T2> &map)
    {
        std::string os;

        for (auto &it : map)
            os += std::to_string(it.first) + std::string(" : ") + std::to_string(it.second) + std::string("\n");

        return os;
    }

    template<typename T>
    static T _print(const T& p)
    {
        return p;
    }

    template<class... Args>
    static void print(Args &&... args)
    {
        int i = 0;
        auto arguments = std::make_tuple(std::forward<Args>(args)...);

        for_each(arguments, [&](const auto &x)
        {
            if(i > 0)
            {
                Print{} << ", ";

            }
            Print{} << _print(x);

            ++i;
        });

        Print{} << std::endl;
    }

private:
    static std::mutex _mutexPrint;
};

#endif //SITEST_SIRENPRINT_HPP