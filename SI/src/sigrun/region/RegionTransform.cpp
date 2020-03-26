
#include "RegionTransform.hpp"

/**
\brief default constructor initializing instance variables to default values
\details
    Default constructor.
    Initializes all matrix objects to identity matrices.
    Sets cumulative angle to 0

\see d_transform
\see d_translation
\see d_rotation
\see d_scale
\scale d_angle
*/
RegionTransform::RegionTransform():
    d_transform(1),
    d_translation(1),
    d_positive_rotation_origin(1),
    d_negative_rotation_origin(1),
    d_rotation(1),
    d_scale(1),
    d_angle(0)
{}

/**
\brief default destructor
\details
    Default destructor.
*/
RegionTransform::~RegionTransform()
{}

/**
\brief central function to update transformation matrix with new, relative translation, relative rotation and absolute scale values
\details
    Updates translation matrix T, rotation matrix R, and scale matrix S according to the given parameters.
    Too small angle increments are ignored to save computations of required trigonometric functions.
    Computes a the new transformation matrix according to T * R * S.

@param translation a constant reference to a glm::vec2 datastructure containing the new, relative translation of the parent contour
@param angle a float containing the new relative angle of the parent contour according to x-axis
@param scale a float containing the new absolute scale factor of the contour

\see d_translation
\see d_angle
\see d_rotation
\see d_scale
\see d_transform
 */
void RegionTransform::update(const glm::vec2& translation, float angle, float scale, const glm::vec2& rotation_origin)
{
    d_translation[0][2] = translation.x;
    d_translation[1][2] = translation.y;

    d_positive_rotation_origin[0][2] = rotation_origin.x;
    d_positive_rotation_origin[1][2] = rotation_origin.y;

    d_negative_rotation_origin[0][2] = -rotation_origin.x;
    d_negative_rotation_origin[1][2] = -rotation_origin.y;

    d_angle += angle;

    if(angle >= 0.1)
    {
        float radians = PI_DIV_180 * d_angle;

        d_rotation[0][0] = std::cos(radians);
        d_rotation[1][0] = -std::sin(radians);
        d_rotation[0][1] = std::sin(radians);
        d_rotation[1][1] = std::cos(radians);
    }

    d_scale[0][0] = scale;
    d_scale[1][1] = scale;

    d_transform = d_translation * d_positive_rotation_origin* d_rotation * d_negative_rotation_origin * d_scale;
}

/**
@return a constant reference to a glm::mat3x3  object containing the current transformation matrix

\see d_transform
*/
const glm::mat3x3& RegionTransform::transform()
{
    return d_transform;
}

/**
\brief overloading of [] operator
\details
    Overloading of [] operator.
    Makes it easier to use the transformation matrix stored in this class.
    This function returns a constant glm::vec3 reference which itself is subscriptable with the [] operator.

@param index an integer containing the index of the row of the transformation matrix to be retrieved.

@return a constant reference of glm::vec3 object containing the queried row of the transformation matrix

\see d_transform
*/
const glm::vec3& RegionTransform::operator[](uint32_t index)
{
    return d_transform[index];
}
