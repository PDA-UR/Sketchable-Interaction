
#include <core/math/si_math.hpp>
#include "CollisionMask.hpp"

CollisionMask::CollisionMask(int width, int height, const std::vector<Vector2<float>> &contour)
        : d_width(width), d_height(height), d_values_size((int) (width * height / sizeof(int)) + 1)
{
    d_values = (int*) malloc(d_values_size * sizeof(int));

    for(int i = 0; i < d_values_size; i++)
        d_values[i] = 0;

    create(contour);
}

CollisionMask::~CollisionMask()
{
    free(d_values);
    d_values = nullptr;
    d_values_size = 0;
}

void CollisionMask::create(const std::vector<Vector2<float>> &contour)
{
    std::vector<std::vector<Vector2<float>>> edges;
    std::vector<Vector2<float>> c = contour;

    if(c.size() > 2)
    {
        c.push_back(c[0]);
        retrieve_edges(edges, c);
        fill_contour_scanline(Vector2<float>::y_min(contour), Vector2<float>::y_max(contour), edges, c);
    }
}

void CollisionMask::retrieve_edges(std::vector<std::vector<Vector2<float>>> &dest,
                                         const std::vector<Vector2<float>> &contour)
{
    int i;

    for(i = 1; i < contour.size(); i++)
        dest.emplace_back(std::vector<Vector2<float>> {contour[i - 1], contour[i]});

    dest.emplace_back(std::vector<Vector2<float>> {contour[i], contour[0]});
}

void CollisionMask::fill_contour_scanline(int ymin, int ymax, const std::vector<std::vector<Vector2<float>>> &edges,
                                           const std::vector<Vector2<float>> &contour)
{
    for(int k = ymax; k > ymin; k--)
    {
        std::vector<Vector2<float>> intersections;
        std::vector<std::vector<Vector2<float>>> pairs_to_fill;
        std::vector<Vector2<float>> scanline { Vector2<float>(0, k), Vector2<float>(d_width, k) };

        for(const auto& edge : edges)
        {
            const Vector2<float>* v = Vector2<float>::intersect(scanline, edge);

            if(v)
                intersections.push_back(*v);
        }

        std::sort(intersections.begin(), intersections.end(), [&](const Vector2<float>& v, const Vector2<float>& u) { return v.x() < u.x(); });

        for(int n = 1; n < intersections.size(); n += 2)
            pairs_to_fill.emplace_back(std::vector<Vector2<float>> { intersections[n - 1], intersections[n] });

        for(auto& pair : pairs_to_fill)
            for (int x = pair[0].x(); x <= pair[1].x(); x++)
                set_bit(Vector2<float>(x, pair[1].y()));
    }
}

void CollisionMask::set_bit(const Vector2<float> &v)
{
    set_bit(d_width * v.y() + v.x());
}

void CollisionMask::set_bit(int i)
{
    d_values[i / sizeof(int)] |= 1 << (i % sizeof(int));
}

void CollisionMask::clear_bit(const Vector2<float> &v)
{
    clear_bit(d_width * v.y() + v.x());
}

void CollisionMask::clear_bit(int i)
{
    d_values[i / sizeof(int)] &= ~(1 << (i % sizeof(int)));
}

bool CollisionMask::test_bit(const Vector2<float> &v) const
{
    return test_bit(d_width * v.y() + v.x());
}

bool CollisionMask::test_bit(int k) const
{
    return ((d_values[k / sizeof(int)] & (1 << (k % sizeof(int)))) != 0);
}

bool CollisionMask::operator[](int i) const
{
    return test_bit(i);
}
