

#ifndef SITEST_REGIONTRANSFORM_HPP
#define SITEST_REGIONTRANSFORM_HPP

#include <glm/glm.hpp>
#include <vector>
#include <sigrun/SIObject.hpp>

/**
\brief quivalent to M_PI / 180.0
\details
    Equivalent to M_PI / 180.0.
    Can be used to convert angles given in degrees to radians.
*/
#define PI_DIV_180 (float) 0.0174532925199

/**
\class RegionTransform
\brief RegionTransform class storing the relative translation, rotation and scale of a contour.
\details
    This class stores the relative translation, rotation and scale of a contour.
    The initial contour remains unchanged and change in one of those three aspects does mutate this transform but not the initial contour.
    The translation, rotation and scale are stored as a 3x3 transformation matrix.
    The transformation matrix is stored ROW MAJOR and requires LEFT or PRE-Multiplication.
    Therefore, multiplications with points look such as : p * T, where p is a point and T is the transformation matrix.
    Due to matrix multiplications being not commutative, T * p will not yield desired results.

\see d_translation
\see d_rotation
\see d_scale
\see d_transform
\see d_angle
*/
class RegionTransform: public SIObject
{ SIGRUN
public:
    RegionTransform();
    ~RegionTransform();

    void update(const glm::vec2& translation=glm::vec2(0, 0), float angle=0.0, float scale=1.0, const glm::vec2& rotation_origin=glm::vec2(0, 0));
    const glm::mat3x3& transform();
    const glm::vec3& operator[](uint32_t index);

private:
    /**
        3x3 matrix object storing the relative translation of a contour
    */
    glm::mat3x3 d_translation;

    /**
        3x3 matrix object storing the positive values of a point to be used as rotation origin
        used for rotating around an arbitrary point
    */
    glm::mat3x3 d_positive_rotation_origin;

    /**
        3x3 matrix object storing the negative values of a point to be used as rotation origin
        used for rotating around an arbitrary point
    */
    glm::mat3x3 d_negative_rotation_origin;

    /**
        3x3 matrix object storing the relative rotation of a contour
    */
    glm::mat3x3 d_rotation;

    /**
        3x3 matrix object storing the scale of a contour
    */
    glm::mat3x3 d_scale;

    /**
        3x3 matrix object storing the product of translation matrix * rotation matrix * scale matrix

        \see d_translation
        \see d_rotation
        \see d_scale
    */
    glm::mat3x3 d_transform;

    /**
        float storing the cumulative rotation angle
    */
    float d_angle;
};


#endif //SITEST_REGIONTRANSFORM_HPP
