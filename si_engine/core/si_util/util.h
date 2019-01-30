//
// Created by juergen on 29/01/19.
//

#ifndef CORE_UTIL_H
#define CORE_UTIL_H

#include <algorithm>
#include <vector>

namespace si
{
    template <typename T>
    int contains( const std::vector<T> &v, const T &x)
    {
        auto it = std::find(v.begin(), v.end(), x);

        if (it != v.end())
            return (int) (it - v.begin());

        return  -1;
    }
}

#endif //CORE_UTIL_H
