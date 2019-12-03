

#ifndef SITEST_CAMERA2D_HPP
#define SITEST_CAMERA2D_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera2D
{
public:

    Camera2D();
    ~Camera2D();

    void initialize(int width, int height);
    void update();
    void set_position(const glm::vec2 &position);

    const glm::vec2 &position();

    void set_scale(float scale);
    float scale();

    const glm::mat4 &camera_matrix() const;

private:
    int d_width, d_height;

    bool d_needs_update;
    glm::vec2 d_position;
    glm::mat4 d_camera_matrix;
    glm::mat4 d_ortho_matrix;

    float d_scale;
};


#endif //SITEST_CAMERA2D_HPP
