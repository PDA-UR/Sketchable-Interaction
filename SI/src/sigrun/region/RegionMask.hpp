

#ifndef SITEST_REGIONMASK_HPP
#define SITEST_REGIONMASK_HPP

#include <vector>
#include <glm/glm.hpp>


/**
\class RegionMask
\brief RegionMask class which stores a bit array used for true collision testing
\details
    Functionality\n

    RegionMask class storing a bit array as std::vector<bool>.
    std::vector<bool> has a special implementation where its bool is stored in exactly one bit.
    See: https://en.cppreference.com/w/cpp/container/vector_bool
    This vector has the size of width * height of the AABB of the contour of the parent region.
    The array is filled with ones and zeroes according to a scanline algorithm.
    Every pixel which is part of the parent Region is set to one in that way.
    Others are left at 0.
    The array is relatively accessed according to the top left corner of that AABB.
    \n
    Rationale:\n

    The use of the AABB allows for creating a secondary coordinate system which is translated relatively to the parent coordinate system (canvas coordinate system).
    Therefore, each point which is to be tested with the mask is subtracted by the position vector of the AABB.
    In this way, that point in the canvas coordinate system is converted to the mask coordinate system.
    This leads to querying collosion occurrences relatively to the AABB.
    Through that, simple region translation does not require recomputation of the mask.
    Instead, the internal AABB is translated the same amount and the coordinate system conversion provides correct collision detection behaviour.
    \n
*/
class RegionMask
{
public:
    RegionMask(int canvas_width, int canvas_height, const std::vector<glm::vec3>& contour, const std::vector<glm::vec3>& aabb);
    RegionMask(const RegionMask& rm);
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
    /**
        int containing the width of the canvas
    */
    int d_canvas_width;

    /**
        int containing the height of the canvas
    */
    int d_canvas_height;

    /**
        int containing the x coordinate of the top left corner of the parent AABB
    */
    int d_tlc_aabb_x;

    /**
        int containing the y coordinate of the top left corner of the parent AABB
    */
    int d_tlc_aabb_y;

    /**
        int containing the x coordinate of the bottom right corner of the parent AABB
    */
    int d_brc_aabb_x;

    /**
         int containing the y coordinate of the bottom right corner of the parent AABB
     */
    int d_brc_aabb_y;

    /**
        int containing the width of the AABB
    */
    int d_width_aabb;

    /**
        int containing the height of the AABB
    */
    int d_height_aabb;

    /**
        std::vector of bools containing the bit array used for testing for collisions
    */
    std::vector<bool> d_values;

    void scanlinefill(const std::vector<glm::vec3>& contour, const std::vector<glm::vec3>& aabb);

    friend class SIGRunRegionMaskTest;
};


#endif //SITEST_REGIONMASK_HPP
