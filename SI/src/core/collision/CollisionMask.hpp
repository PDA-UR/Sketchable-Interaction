

#ifndef SI_COLLISIONMASK_HPP
#define SI_COLLISIONMASK_HPP

#include <vector>
#include <algorithm>
#include "core/math/si_math.hpp"

class CollisionMask
{
public:
    CollisionMask(int width, int height, const std::vector<Vector2<float>> &contour);
    ~CollisionMask();

    void set_bit(const Vector2<float> &v);
    void set_bit(int i);
    void clear_bit(const Vector2<float> &v);
    void clear_bit(int i);
    bool test_bit(const Vector2<float> &v) const;
    bool test_bit(int k) const;
    bool operator [](int i) const;

private:
    int d_width, d_height;

    void create(const std::vector<Vector2<float>> &contour);
    void retrieve_edges(std::vector<std::vector<Vector2<float>>> &dest, const std::vector<Vector2<float>> &contour);
    void fill_contour_scanline(int ymin, int ymax, const std::vector<std::vector<Vector2<float>>> &edges,
                               const std::vector<Vector2<float>> &contour);

    int* d_values;
    int d_values_size = 0;
};


#endif //SI_COLLISIONMASK_HPP
