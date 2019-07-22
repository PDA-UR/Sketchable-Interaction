/*
 *
 * Requires C++14 Standard
 *
 * */


#include <tuple>
#include <iostream>

template<typename TupleType, typename FunctionType>
void for_each(TupleType&&, FunctionType, std::integral_constant<size_t, std::tuple_size<typename std::remove_reference<TupleType>::type >::value>) {}

template<std::size_t I, typename TupleType, typename FunctionType , typename = typename std::enable_if<I!=std::tuple_size<typename std::remove_reference<TupleType>::type>::value>::type >
void for_each(TupleType&& t, FunctionType f, std::integral_constant<size_t, I>)
{
    f(std::get<I>(t));
    for_each(std::forward<TupleType>(t), f, std::integral_constant<size_t, I + 1>());
}

template<typename TupleType, typename FunctionType>
void for_each(TupleType&& t, FunctionType f)
{
    for_each(std::forward<TupleType>(t), f, std::integral_constant<size_t, 0>());
}

void print()
{
    std::cout << std::endl;
}

template <class... Args>
void print(Args&&... args)
{
    auto arguments = std::make_tuple(std::forward<Args>(args)...);

    for_each(arguments, [](const auto &x)
    {
        std::cout << x << " ";
    });

    print();
}



int main()
{
    print(2, 4, 6, 8);
    print();
    print("Hello", 1337, 3.14, "equals PI", "WTF");
}
