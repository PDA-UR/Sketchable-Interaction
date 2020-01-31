

#ifndef SITEST_UTIL_HPP
#define SITEST_UTIL_HPP

#include <vector>

#define TO_RADIANS(x) (x * 0.01745329251) // pi / 180
#define TO_DEGREES(x) (x * 57.2957795130) // 180 / pi

#define VEC_CONTAINS(vec, elem) (std::find(vec.begin(), vec.end(), elem) != vec.end())
#define MAP_HAS_KEY(map, key) (map.find(key) != map.end())

#endif //SITEST_UTIL_HPP
