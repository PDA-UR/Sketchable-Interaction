

#ifndef SI_PRINT_HPP
#define SI_PRINT_HPP
#include <boost/python.hpp>
#include <tuple>
#include <iostream>
#include <map>
#include "context/Layer.hpp"

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

    static void print(Layer* layer, bool new_line= false)
    {
        if(new_line)
            std::cout << "Layer with id: " << layer->id() << std::endl;
        else
            std::cout << "Layer with id: " << layer->id() << " ";
    }

    template<typename T>
    static void print(const std::vector<T>& list)
    {
        int i = 0;

        for(i = 0; i < list.size() - 1; i++)
            print(list[i]);

        print(list[i], true);
    }

    template <typename T>
    static void print(const std::vector<std::vector<T>>& list_of_lists)
    {
        for(auto& list : list_of_lists)
        {
            int i = 0;

            for(i = 0; i < list.size() - 1; i++)
                print(list[i]);

            print(list[i], true);
        }
    }

    template <typename T1, typename T2>
    static void print(const std::map<T1, T2>& map)
    {
        print("Map: ");
        auto it = map.begin();

        for(it = map.begin(); it != map.end(); it++)
        {
            print("(Key: ");
            print(it->first);
            print(", Value: ");
            print(it->second);
            print(")");
        }
    }
};

#endif //SI_PRINT_HPP
