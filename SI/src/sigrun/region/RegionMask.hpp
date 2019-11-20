

#ifndef SITEST_REGIONMASK_HPP
#define SITEST_REGIONMASK_HPP

#include <vector>
#include <glm/glm.hpp>

class RegionMask
{
public:
    RegionMask(int canvas_width, int canvas_height, const std::vector<glm::vec3>& contour, const std::vector<glm::vec3>& aabb);
    ~RegionMask();

    int size() const;

    void set_bit(int i);
    void set_bit(const glm::vec3& v);

    void clear_bit(int i);
    void clear_bit(const glm::vec3& v);

    int width() const;
    int height() const;

    void move(const glm::vec2& v);

    bool operator[](int i) const;
    bool operator[](const glm::vec3& v) const;

private:
    int d_canvas_width;
    int d_canvas_height;

    int d_tlc_aabb_x;
    int d_tlc_aabb_y;
    int d_brc_aabb_x;
    int d_brc_aabb_y;
    int d_width_aabb;
    int d_height_aabb;

    std::vector<bool> d_values;

    void scanlinefill(const std::vector<glm::vec3>& contour, const std::vector<glm::vec3>& aabb);

    friend class SIGRunRegionMaskTest;
};


#endif //SITEST_REGIONMASK_HPP
