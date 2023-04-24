#include <debug/Print.hpp>
#include "Subscriber.hpp"

Subscriber::Subscriber():
    Node("Subscriber"),
    sp_subscription(this->create_subscription<ros2_si_interface::msg::VTKSIObject>("/vtk_si_interface", 10, std::bind(&Subscriber::object_callback, this, std::placeholders::_1)))

{}

void Subscriber::object_callback(ros2_si_interface::msg::VTKSIObject::SharedPtr message)
{
    std::vector<glm::vec3> shape;
    to_shape(shape, message->geometry);
    glm::vec4 color(message->color[0], message->color[1], message->color[2], 255);
    TangibleObjectMessage msg(message->id, shape, message->plugin, message->x, message->y, color, message->click, message->drag,
                              message->dblclick, message->touch, message->alive, message->links, message->tracker_dimension_x,
                              message->tracker_dimension_y);
    msg.send();
}

void Subscriber::to_shape(std::vector<glm::vec3>& out, const std::vector<float> &raw)
{
    out.resize(raw.size() / 2);
    for(int i = 0; i < raw.size(); i+=2)
    {
        out[i / 2].x = raw[i];
        out[i / 2].y = raw[i+1];
    }
}
