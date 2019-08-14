
#include <core/math/si_math.hpp>
#include "CollisionMask.hpp"

namespace SI
{
    CollisionMask::CollisionMask(int width, int height, const std::vector<glm::vec2>& contour)
            : d_width(width), d_height(height), d_values_size((int) (width * height / sizeof(int)) + 1)
    {
        d_values = (int *) malloc(d_values_size * sizeof(int));

        for (int i = 0; i < d_values_size; i++)
            d_values[i] = 0;

        create(contour);
    }

    CollisionMask::~CollisionMask()
    {
        free(d_values);
        d_values = nullptr;
        d_values_size = 0;
    }

    void CollisionMask::create(const std::vector<glm::vec2>& contour)
    {
        std::vector<std::vector<glm::vec2>> edges;
        std::vector<glm::vec2> c = contour;

        if (c.size() > 2)
        {
            c.push_back(c[0]);
            retrieve_edges(edges, c);
            fill_contour_scanline(SIMath::y_min(contour), SIMath::y_max(contour), edges, c);
        }
    }

    void CollisionMask::retrieve_edges(std::vector<std::vector<glm::vec2>> &dest,
                                       const std::vector<glm::vec2>& contour)
    {
        int i;

        for (i = 1; i < contour.size(); i++)
            dest.emplace_back(std::vector<glm::vec2> {contour[i - 1], contour[i]});

        dest.emplace_back(std::vector<glm::vec2> {contour[i], contour[0]});
    }

    void CollisionMask::fill_contour_scanline(float ymin, float ymax, const std::vector<std::vector<glm::vec2>>& edges,
                                              const std::vector<glm::vec2>& contour)
    {
        for (int k = ymax; k > ymin; k--)
        {
            std::vector<glm::vec2> intersections;
            std::vector<std::vector<glm::vec2>> pairs_to_fill;
            std::vector<glm::vec2> scanline{glm::vec2(0, k), glm::vec2(d_width, k)};

            for (const auto &edge : edges)
            {
                const glm::vec2 *v = SIMath::intersect(scanline, edge);

                if (v)
                    intersections.push_back(*v);
            }

            std::sort(intersections.begin(), intersections.end(), [&](const glm::vec2 &v, const glm::vec2 &u)
            { return v.x < u.x; });

            for (int n = 1; n < intersections.size(); n += 2)
                pairs_to_fill.emplace_back(std::vector<glm::vec2>{intersections[n - 1], intersections[n]});

            for (auto &pair : pairs_to_fill)
                for (int x = pair[0].x; x <= pair[1].x; x++)
                    set_bit(glm::vec2(x, pair[1].y));
        }
    }

    void CollisionMask::set_bit(const glm::vec2& v)
    {
        set_bit(d_width * v.y + v.x);
    }

    void CollisionMask::set_bit(int i)
    {
        d_values[i / sizeof(int)] |= 1 << (i % sizeof(int));
    }

    void CollisionMask::clear_bit(const glm::vec2 &v)
    {
        clear_bit(d_width * v.y + v.x);
    }

    void CollisionMask::clear_bit(int i)
    {
        d_values[i / sizeof(int)] &= ~(1 << (i % sizeof(int)));
    }

    bool CollisionMask::test_bit(const glm::vec2 &v) const
    {
        return test_bit(d_width * v.y + v.x);
    }

    bool CollisionMask::test_bit(int k) const
    {
        return ((d_values[k / sizeof(int)] & (1 << (k % sizeof(int)))) != 0);
    }

    bool CollisionMask::operator[](int i) const
    {
        return test_bit(i);
    }
}