

#ifndef SI_COLLISIONMASK_HPP
#define SI_COLLISIONMASK_HPP

#include <vector>
#include <algorithm>
#include "core/math/si_math.hpp"
#include <glm/glm.hpp>

namespace SI
{
    class CollisionMask
    {
    public:
        CollisionMask(int width, int height, const std::vector<glm::vec2>& contour);

        ~CollisionMask();

        void set_bit(const glm::vec2 &v);
        void set_bit(int i);
        void clear_bit(const glm::vec2 &v);
        void clear_bit(int i);
        bool test_bit(const glm::vec2 &v) const;
        bool test_bit(int k) const;
        bool operator[](int i) const;

    private:
        int d_width, d_height;

        void create(const std::vector<glm::vec2>& contour);
        void retrieve_edges(std::vector<std::vector<glm::vec2>> &dest, const std::vector<glm::vec2>& contour);
        void fill_contour_scanline(float ymin, float ymax, const std::vector<std::vector<glm::vec2>> &edges, const std::vector<glm::vec2>& contour);

        int *d_values;
        int d_values_size = 0;
    };
}

#endif //SI_COLLISIONMASK_HPP
