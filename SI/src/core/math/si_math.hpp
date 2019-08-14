
#ifndef SI_SI_MATH_HPP
#define SI_SI_MATH_HPP

#include <vector>
#include <algorithm>
#include <glm/glm.hpp>

namespace SI
{
    class SIMath
    {
    public:
        static float y_min(const std::vector<glm::vec2> &contour)
        {
            return std::min_element(contour.begin(), contour.end(), [&](const glm::vec2 &v, const glm::vec2 &u)
            { return v.y < u.y; }).base()->y;
        }

        static float y_max(const std::vector<glm::vec2> &contour)
        {
            return std::max_element(contour.begin(), contour.end(), [&](const glm::vec2 &v, const glm::vec2 &u)
            { return v.y < u.y; }).base()->y;
        }

        static glm::vec2* intersect(const std::vector<glm::vec2> &l, const std::vector<glm::vec2> &m)
        {
            float epsilon = 0.0001;

            const glm::vec2 &p1 = l[0], &p2 = l[1], &p3 = m[0], &p4 = m[1];

            float x1 = p1.x, x2 = p2.x, x3 = p3.x, x4 = p4.x;
            float y1 = p1.y, y2 = p2.y, y3 = p3.y, y4 = p4.y;

            float d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

            if (d == 0)
                return nullptr;

            float pre = (x1 * y2 - y1 * x2);
            float post = (x3 * y4 - y3 * x4);

            float x = (pre * (x3 - x4) - (x1 - x2) * post) / d;
            float y = (pre * (y3 - y4) - (y1 - y2) * post) / d;

            if (x < (std::min(x1, x2) - epsilon) || x > (std::max(x1, x2) + epsilon) ||
                x < (std::min(x3, x4) - epsilon) || x > (std::max(x3, x4) + epsilon))
                return nullptr;

            if (y < (std::min(y1, y2) - epsilon) || y > (std::max(y1, y2) + epsilon) ||
                y < (std::min(y3, y4) - epsilon) || y > (std::max(y3, y4) + epsilon))
                return nullptr;

            return new glm::vec2(x, y);
        }
    };
}

#endif //SI_SI_MATH_HPP
