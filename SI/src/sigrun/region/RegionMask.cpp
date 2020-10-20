
//#include <debug/Print.hpp>
#include <algorithm>
#include "RegionMask.hpp"

#include <iostream>
#include <sigrun/log/Log.hpp>

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
RegionMask::RegionMask(uint32_t canvas_width, uint32_t canvas_height, const std::vector<glm::vec3> &contour, const std::vector<glm::vec3> &aabb):
    d_canvas_width(canvas_width),
    d_canvas_height(canvas_height),
    d_tlc_aabb_x(aabb[0].x),
    d_tlc_aabb_y(aabb[0].y),
    d_brc_aabb_x(aabb[2].x),
    d_brc_aabb_y(aabb[2].y),
    d_move_x(0),
    d_move_y(0),
    d_width_aabb(d_brc_aabb_x - d_tlc_aabb_x),
    d_height_aabb(d_brc_aabb_y - d_tlc_aabb_y),
    d_values(std::vector<bool>(d_width_aabb * d_height_aabb))
{
    scanlinefill(contour, aabb);

//    for(int i = 0; i < d_width_aabb; ++i)
//    {
//        for(int k = 0; k < d_height_aabb; ++k)
//        {
//            std::cout << (int) d_values[d_width_aabb * k + i];
//        }
//
//        std::cout << std::endl;
//    }
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
        d_tlc_aabb_x(rm.d_tlc_aabb_x),
        d_tlc_aabb_y(rm.d_tlc_aabb_y),
        d_brc_aabb_x(rm.d_brc_aabb_x),
        d_brc_aabb_y(rm.d_brc_aabb_y),
        d_move_x(0),
        d_move_y(0),
        d_width_aabb(rm.d_width_aabb),
        d_height_aabb(rm.d_height_aabb),
        d_values(rm.d_values)
{}

/**
\brief default destructor
*/
RegionMask::~RegionMask()
{

}

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
    int32_t index = d_width_aabb * ((int32_t) v.y - (d_tlc_aabb_y + d_move_y)) + (int32_t) v.x - (d_tlc_aabb_x + d_move_x);

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
    int32_t index = d_width_aabb * ((int32_t) v.y - (d_tlc_aabb_y + d_move_y)) + (int32_t) v.x - (d_tlc_aabb_x + d_move_x);

    if(index > -1 && index < d_values.size())
        this->clear_bit(index);
}

/**
@return the width of the AABB of the parent Region

\see d_width_aabb
*/
uint32_t RegionMask::width() const
{
    return d_width_aabb;
}

/**
@return the height of the AABB of the parent Region

\see d_height_aabb
*/
uint32_t RegionMask::height() const
{
    return d_height_aabb;
}

void RegionMask::rebuild(const std::vector<glm::vec3> &contour, const std::vector<glm::vec3> &aabb)
{
    d_values.clear();
    d_values.reserve(d_width_aabb * d_height_aabb);

    scanlinefill(contour, aabb);
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
    if(i > -1 && i < d_values.size())
        return d_values[i];

    return false;
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
    int index = d_width_aabb * ((int) v.y - (d_tlc_aabb_y + d_move_y)) + (int) v.x - (d_tlc_aabb_x + d_move_x);

    if(index > -1 && index < d_values.size())
        return d_values[index];

    return false;
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
void RegionMask::scanlinefill(const std::vector<glm::vec3>& contour, const std::vector<glm::vec3>& aabb)
{
    int nodes;
    int nodeX[256];

    //  Loop through the rows of the image.
    for (int y = d_tlc_aabb_y; y < d_brc_aabb_y; y++)
    {
        //  Build a list of nodes.
        nodes = 0;
        for (int i = 0, k = contour.size() - 2 - 1; i < contour.size() - 2; i++)
        {
            if (contour[i].y < (double) y && contour[k].y >= (double) y || contour[k].y < (double) y && contour[i].y >= (double) y)
                nodeX[nodes++] = (int) (contour[i].x + (y - contour[i].y) / (contour[k].y - contour[i].y) * (contour[k].x - contour[i].x));

            k = i;
        }

        //  Sort the nodes, via a simple “Bubble” sort.

        int swap;
        for (int i = 0; i < nodes - 1;)
        {
            if (nodeX[i] > nodeX[i + 1])
            {
                swap = nodeX[i];
                nodeX[i] = nodeX[i + 1];
                nodeX[i + 1] = swap;
                if (i) i--;
            }
            else
            {
                i++;
            }
        }

        //  Fill the pixels between node pairs.
        for (int i = 0; i < nodes; i += 2)
        {
            if (nodeX[i] >= d_canvas_width)
                break;

            if (nodeX[i + 1] > 0)
            {
                if (nodeX[i] < 0)
                    nodeX[i] = 0;

                if (nodeX[i + 1] > d_canvas_width)
                    nodeX[i + 1] = d_canvas_width;

                for (int x = nodeX[i]; x < nodeX[i + 1]; x++)
                    this->set_bit(glm::vec3(x, y, 1));
            }
        }
    }
}
