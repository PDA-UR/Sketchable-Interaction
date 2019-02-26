//
// Created by juergen on 29/01/19.
//

#ifndef CORE_UTIL_H
#define CORE_UTIL_H

#include <algorithm>
#include <vector>
#include <memory>

namespace si
{
    template <typename T>
    int contains(const std::vector<T> &v, const T &x)
    {
        auto it = std::find(v.begin(), v.end(), x);

        return (it != v.end()) ? ((int) (it - v.begin())) : -1;
    }
}

#endif //CORE_UTIL_H
