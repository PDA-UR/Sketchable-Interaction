
#ifndef SIGRUN_PRINT_HPP
#define SIGRUN_PRINT_HPP

#include <boost/python.hpp>
#include <mutex>
#include <tuple>
#include <iostream>
#include <map>
#include <sstream>

namespace bp = boost::python;
//
//std::ostream &operator<<(std::ostream &os, const std::vector<T> &v)
//{
//    os << "[";
//
//    for (int i = 0; i < v.size(); ++i)
//    {
//        os << v[i];
//
//        if (i != v.size() - 1)
//            os << ", ";
//    }
//
//    os << "]";
//
//    return os;
//}
//
//template<typename T1, typename T2>
//std::ostream &operator<<(std::ostream &os, const std::map<T1, T2> &map)
//{
//    for (auto &it : map)
//        os << it.first << " : " << it.second << "\n";
//
//    return os;
//}
//
//std::ostream &operator<<(std::ostream &os, const bp::object& pobj)
//{
//    return os << bp::extract<std::string>(bp::str(pobj))();
//}

class Print : public std::ostringstream
{
public:
    Print() = default;
    ~Print()
    {
        std::lock_guard<std::mutex> guard(_mutexPrint);
        std::cout << this->str();
    }

    template<typename T>
    static void print(const std::vector<std::vector<T>> &v)
    {
        std::ostringstream os;

        os << "[";

        for (int i = 0; i < v.size(); ++i)
        {
            os << "[";
            for(int k = 0; k < v[i].size(); k++)
            {
                os << v[i][k];

                if (k != v.size() - 1)
                    os << ", ";
            }
            os << "]";
        }

        os << "]";

        print(os.str());
    }

    template<typename T>
    static void print(const std::vector<T> &v)
    {
        std::ostringstream os;

        os << "[";

        for (int i = 0; i < v.size(); ++i)
        {
            os << v[i];

            if (i != v.size() - 1)
                os << ", ";
        }

        os << "]";

        print(os.str());
    }

    template <typename T1, typename T2>
    static void print(const std::map<T1, T2> &map)
    {
        std::ostringstream os;

        for (auto &it : map)
            os << it.first << " : " << it.second << "\n";

        print(os.str());
    }

    template <typename T>
    static void print(const T& arg)
    {
        Print{} << arg << std::endl;
    }

private:
    static std::mutex _mutexPrint;
};

#endif //SITEST_SIRENPRINT_HPP