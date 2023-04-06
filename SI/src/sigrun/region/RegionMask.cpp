
#include <algorithm>
#include "RegionMask.hpp"

#if !defined(Q_MOC_RUN)
#include <tbb/parallel_for.h>
#include <tbb/tbb.h>
#endif

#include <iostream>
#include <fstream>

#define NUM_DEFAULT_SCANLINES 24

#define MASK_DEBUG \
++d_num_called; \
if(d_num_called > NUM_DEFAULT_SCANLINES) \
{ \
    std::ofstream f; \
    f.open("build/test" + std::to_string(d_num_called) + "_" + name + ".txt"); \
    for (int i = 0; i < d_canvas_height; ++i) \
    { \
        for (int k = 0; k < d_canvas_width; ++k) \
            f << (int) d_values[d_canvas_width * i + k]; \
        f << "\n"; \
    } \
    f.close();\
}

int RegionMask::d_num_called = 0;

/**
\brief constructor of the RegionMask class
\details
    Constructor of the RegionMask class.
    Initializes all datastructures required to maintain a RegionMask for CollisionDetection according to parameters.
    Performs scanline algorithm for generation of the actual mask relatively to AABB of parent region.

@param canvas_width int containing the width of the canvas
@param canvas_height int containing the height of the canvas
@param contour constant reference to a std::vector object containing glm::vec3 objects containing all the points of the contour of the parent Region
@param aabb constant reference to a std::vector object containing glm::vec3 objects containing the four points of the AABB of the parent Region

\see d_canvas_width
\see d_canvas_height
\see d_tlc_aabb_x
\see d_tlc_aabb_y
\see d_brc_aabb_x
\see d_brc_aabb_y
\see d_width_aabb
\see d_height_aabb
\see d_values
 */
RegionMask::RegionMask(uint32_t canvas_width, uint32_t canvas_height, const std::vector<glm::vec3> &contour):
    d_canvas_width(canvas_width),
    d_canvas_height(canvas_height),
    d_move_x(0),
    d_move_y(0),
    d_values(std::vector<bool>(d_canvas_width * d_canvas_height))
{
    scanlinefill(contour);
}
/**
\brief copy constructor

@param rm the constant reference to a RegionMask object to be copied

\see d_canvas_width
\see d_canvas_height
\see d_tlc_aabb_x
\see d_tlc_aabb_y
\see d_brc_aabb_x
\see d_brc_aabb_y
\see d_width_aabb
\see d_height_aabb
\see d_values
*/
RegionMask::RegionMask(const RegionMask &rm):
        d_canvas_width(rm.d_canvas_width),
        d_canvas_height(rm.d_canvas_height),
        d_move_x(0),
        d_move_y(0),
        d_values(rm.d_values)
{}

/**
\brief default destructor
*/
RegionMask::~RegionMask() = default;

/**
\brief retrieve the size of the mask datastructure
\details
    Retrieves the size of the mask datastructure.
    This datastructure is called d_values.

@return the size of d_values

\see d_values
*/
uint32_t RegionMask::size() const
{
    return d_values.size();
}

/**
\brief set the bit at index i of d_values to one/true
\details
    Sets the bit at index i of d_values to one or true according to a bounds check.

@param i int which contains the index of the bit to be set to one/true in d_values

\see d_values
*/
void RegionMask::set_bit(int32_t i)
{
    if(i > -1 && i < d_values.size())
        d_values[i] = true;
    else
        d_values[i] = false;
}

/**
\brief set the bit at point v to one/true in d_values
\details
    Sets the bit at point v of d_values to one or true according to a bounds check.
    The bounds check is performed based on the actual index of the bit to be set.
    The actual index is calculated according to AABB_WIDTH * (v.y - AABB_TOP_LEFT_CORNER_Y) + v.x - AABB_TOP_LEFT_CORNER_X
    If the bounds check is negative, the bit will be set to false.

@param v a constant reference to a glm::vec3 object containing the corresponding coordinates of the point to a bit of d_values which is to be set to one or true.

\see d_values
\see d_width_aabb
\see d_tlc_aabb_y
\see d_tlc_aabb_x
 */
void RegionMask::set_bit(const glm::vec3& v)
{
    int32_t index = d_canvas_width * ((int32_t) v.y) + ((int32_t) v.x);

    if(index > -1 && index < d_values.size())
        this->set_bit(index);
}

/**
\brief set the bit at index i of d_values to zero/false
\details
    Sets the bit at index i of d_values to zero or false according to a bounds check.

@param i int which contains the index of the bit to be set to zero/false in d_values

\see d_values
*/
void RegionMask::clear_bit(int32_t i)
{
    if(i > -1 && i < d_values.size())
        d_values[i] = false;
}

/**
\brief set the bit at point v of d_values to zero/false
\details
    Sets the bit at point v of d_values to zero or false according to a bounds check.
    The bounds check is performed based on the actual index of the bit to set.
    The actual index is calculated according to AABB_WIDTH * (v.y - AABB_TOP_LEFT_CORNER_Y) + v.x - AABB_TOP_LEFT_CORNER_X
    If the bounds check is negative, nothing happens.

@param v a constant reference to a glm::vec3 object containing the corresponding coordinates of the point to a bit of d_values which is to be set to zero or false.

\see d_values
\see d_width_aabb
\see d_tlc_aabb_y
\see d_tlc_aabb_x
*/
void RegionMask::clear_bit(const glm::vec3 &v)
{
    int32_t index = d_canvas_width * (int32_t) v.y + (int32_t) v.x;

    if(index > -1 && index < d_values.size())
        this->clear_bit(index);
}

/**
\brief [] operator overloaded for returning the value of d_values at index i
\details
    Overloads the [] operator.
    Retrieves the bool value at index i of d_values.

@param i int containing the index
@return a bool containing whether the queried bit is set or not in d_values

\see d_values
*/
bool RegionMask::operator[](int32_t i) const
{
    return (i > -1 && i < d_values.size()) && d_values[i];
}

/**
\brief [] operator overloaded for returning the value of d_values at point v
\details
    Overloads the [] operator.
    Retrieves the bool value at point v of d_values.
    The actual index is calculated according to AABB_WIDTH * (v.y - AABB_TOP_LEFT_CORNER_Y) + v.x - AABB_TOP_LEFT_CORNER_X.


@param v a constant reference to a glm::vec3 object containing the corresponding coordinates of the point to a bit of d_values which is to be tested whether the queried bit is set or not in d_values.
@return a bool containing whether the queried bit is set or not in d_values

\see d_values
\see d_width_aabb
\see d_tlc_aabb_x
\see d_tlc_aabb_y
*/
bool RegionMask::operator[](const glm::vec3 &v) const
{
    return (*this)[d_canvas_width * ((int32_t) v.y - d_move_y) + ((int32_t) v.x - d_move_x)];
}

/**
\brief update the AABB relations according to desired translation of a parent Region
\details
    Use of RegionMasks occurs relatively to the AABB of its parent Region.
    Is the parent region moved / translated within the canvas, the AABB is also moved or translated.
    Therefore, the RegionMasks is upated according to that translation by storing the new translation parameters.
    In this way, the mask coordinate system is moved within the canvas coordinate system.
    So, after updating the AABB with the new translation values, the mask continues to function, due to its relative dependence on the AABB.
    Therefore, no recomputation is required.

@param v a constant reference to a glm::vec2 object containing the translation vector
*/
void RegionMask::move(const glm::vec2& v)
{
    d_move_x += v.x;
    d_move_y += v.y;
}

/**
\brief perform standard scanline algorithm
\details
    Perform the standard scanline algorithm.
    Use AABB of contour of parent region as scan area.
    Sets bits in d_values to one / true if they are part of the parent Region.
    Otherwise, bits remain at zero / false.

@param contour a constant reference to std::vector object containing glm::vec3 objects depicting the points of the contour of the parent region
@param aabb a constant reference to std::vector object containing glm::vec3 objects depicting the four points of the AABB of the contour of the parent region
*/
void RegionMask::scanlinefill(const std::vector<glm::vec3>& contour)
{
    tbb::parallel_for(tbb::blocked_range<uint32_t>(0, d_canvas_height), [&](const tbb::blocked_range<uint32_t>& r)
    {
        for (auto y = r.begin(); y != r.end(); y++)
        {
            int nodes = 0, nodeX[256];

            build_node_list(nodeX, &nodes, y, contour);
            sort(nodeX, nodes);
            fill(nodeX, nodes, y);
        }
    });
}


void RegionMask::build_node_list(int *out, int *num_out, int y, const std::vector<glm::vec3> &in)
{
    int num_corners = in.size() - 1; // subtract 1 for indexing

    for (int i = 0, k = num_corners - 1; i < num_corners; i++)
    {
        if (in[i].y < (double) y && in[k].y >= (double) y || in[k].y < (double) y && in[i].y >= (double) y)
            out[(*num_out)++] = (int) (in[i].x + (y - in[i].y) / (in[k].y - in[i].y) * (in[k].x - in[i].x));

        k = i;
    }
}

void RegionMask::sort(int in[256], int num_in)
{
    std::sort(in, in + num_in);
}

void RegionMask::fill(int in[256], int num_in, int y)
{
    for (int i = 0; i < num_in; i += 2)
    {
        if (in[i] >= d_canvas_width)
            break;

        if (in[i + 1] > 0)
        {
            in[i] = in[i] < 0 ? 0 : in[i];
            in[i + 1] = in[i + 1] > d_canvas_width ? d_canvas_width : in[i + 1];

            for (int x = in[i] + 1; x < in[i + 1]; x++)
                this->set_bit(glm::vec3(x, y, 1));
        }
    }
}
