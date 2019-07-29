
#ifndef SI_SI_MATH_HPP
#define SI_SI_MATH_HPP

#include <vector>
#include <algorithm>

template <typename T, int D>
class Vector
{

public:
    Vector() { }

    T& operator [] (unsigned int i) { return values[i]; }
    T operator [] (unsigned int i) const { return values[i]; }

private:
    T values[D];
};

template <typename T>
class Vector2 : public Vector<T, 2>
{
public:
    Vector2() {}

    Vector2(const Vector<T, 2>& v)
    {
        (*this)[0] = v[0];
        (*this)[1] = v[1];
    }

    Vector2(T x, T y)
    {
        (*this)[0] = x;
        (*this)[1] = y;
    }

    static int y_min(const std::vector<Vector2<T>>& contour)
    {
        return (T) std::min_element(contour.begin(), contour.end(), [&](const Vector2<T>& v, const Vector2<T>& u) { return v.y() < u.y(); }).base()->y();
    }

    static int y_max(const std::vector<Vector2<T>>& contour)
    {
        return std::max_element(contour.begin(), contour.end(), [&](const Vector2<T>& v, const Vector2<T>& u) { return v.y() < u.y(); }).base()->y();
    }

    int x() const
    {
        return (*this)[0];
    }

    int y() const
    {
        return (*this)[1];
    }

    static Vector2* intersect(const std::vector<Vector2>& l, const std::vector<Vector2>& m)
    {
        float epsilon = 0.0001;

        const Vector2& p1 = l[0], & p2 = l[1], & p3 = m[0], & p4 = m[1];

        float x1 = p1.x(), x2 = p2.x(), x3 = p3.x(), x4 = p4.x();
        float y1 = p1.y(), y2 = p2.y(), y3 = p3.y(), y4 = p4.y();

        float d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

        if (d == 0)
            return nullptr;

        float pre = (x1 * y2 - y1 * x2);
        float post = (x3 * y4 - y3 * x4);

        float x = (pre * (x3 - x4) - (x1 - x2) * post) / d;
        float y = (pre * (y3 - y4) - (y1 - y2) * post) / d;

        if (x < (std::min(x1, x2) - epsilon) || x > (std::max(x1, x2) + epsilon) ||  x < (std::min(x3, x4) - epsilon) || x > (std::max(x3, x4) + epsilon))
            return nullptr;

        if (y < (std::min(y1, y2) - epsilon) || y > (std::max(y1, y2) + epsilon) || y < (std::min(y3, y4) - epsilon)|| y > (std::max(y3, y4) + epsilon))
            return nullptr;

        return new Vector2((T) x, (T) y);
    }
};

#endif //SI_SI_MATH_HPP
