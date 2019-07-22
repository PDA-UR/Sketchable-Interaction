

#ifndef SI_PRINT_HPP
#define SI_PRINT_HPP
#include <boost/python.hpp>
#include <tuple>
#include <iostream>

namespace bp = boost::python;

class Print
{
public:
    template<typename TupleType, typename FunctionType>
    static void for_each(TupleType&&, FunctionType, std::integral_constant<size_t, std::tuple_size<typename std::remove_reference<TupleType>::type >::value>) {}

    template<std::size_t I, typename TupleType, typename FunctionType , typename = typename std::enable_if<I != std::tuple_size<typename std::remove_reference<TupleType>::type>::value>::type >
    static void for_each(TupleType&& t, FunctionType f, std::integral_constant<size_t, I>)
    {
        f(std::get<I>(t));
        for_each(std::forward<TupleType>(t), f, std::integral_constant<size_t, I + 1>());
    }

    template<typename TupleType, typename FunctionType>
    static void for_each(TupleType&& t, FunctionType f)
    {
        for_each(std::forward<TupleType>(t), f, std::integral_constant<size_t, 0>());
    }

    template <class... Args>
    static void print(Args&&... args)
    {
        std::cout << "SI-Engine: ";

        auto arguments = std::make_tuple(std::forward<Args>(args)...);

        for_each(arguments, [](const auto &x)
        {
            print(x);
        });

        std::cout << std::endl;
    }

    template <typename T>
    static void print(const T& arg, bool new_line=false)
    {
        if(new_line)
            std::cout << arg << std::endl;
        else
            std::cout << arg << " ";
    }

    static void print(const bp::object& obj, bool new_line=false)
    {
        if(new_line)
            std::cout << bp::extract<std::string>(bp::str(obj))() << std::endl;
        else
            std::cout << bp::extract<std::string>(bp::str(obj))() << " ";
    }
};

#endif //SI_PRINT_HPP