

#include "Camera2D.hpp"

Camera2D::Camera2D() :
        d_position(0.0f, 0.0f),
        d_camera_matrix(1.0f),
        d_scale(1.0f),
        d_needs_update(true),
        d_width(0),
        d_height(0),
        d_ortho_matrix(1.0f)
{

}

Camera2D::~Camera2D()
{

}

void Camera2D::initialize(int width, int height)
{
    d_width = width;
    d_height = height;
    d_ortho_matrix = glm::ortho(0.0f, (float) d_width, (float) d_height, 0.0f);
    d_camera_matrix = glm::translate(d_ortho_matrix, glm::vec3(d_position.x, d_position.y, 0.0f));

//    glm::vec3 scale(d_scale, d_scale, 0.0f);
//    d_camera_matrix = glm::scale(glm::mat4(1.0f), scale) * d_camera_matrix;
}

void Camera2D::update()
{
//    if (d_needs_update)
//    {
//        glm::vec3 translate(-d_position.x + d_width / 2, -d_position.y + d_height / 2, 0.0f);
//        d_camera_matrix = glm::translate(d_ortho_matrix, translate);
//
//        glm::vec3 scale(d_scale, d_scale, 0.0f);
//        d_camera_matrix = glm::scale(glm::mat4(1.0f), scale) * d_camera_matrix;
//
//        d_needs_update = false;
//    }
}

void Camera2D::set_position(const glm::vec2 &position)
{
    d_needs_update = true;
    d_position = position;
}

const glm::vec2 &Camera2D::position()
{
    return d_position;
}

void Camera2D::set_scale(float scale)
{
    d_needs_update = true;
    d_scale = scale;
}

float Camera2D::scale()
{
    return d_scale;
}

const glm::mat4 &Camera2D::camera_matrix() const
{
    return d_camera_matrix;
}

