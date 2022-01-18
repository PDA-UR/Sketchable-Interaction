#include <debug/Print.hpp>
#include "Subscriber.hpp"

Subscriber::Subscriber(): Node("Subscriber")
{
    sp_subscription = this->create_subscription<ros2_si_interface::msg::VTKSIObject>("topic", 10, std::bind(&Subscriber::object_callback, this, std::placeholders::_1));
}

void Subscriber::object_callback(ros2_si_interface::msg::VTKSIObject::SharedPtr message)
{
    std::vector<glm::vec3> shape(message->geometry.size() / 2);
    to_shape(shape, message->geometry);

    Print::print(shape);
}

void Subscriber::to_shape(std::vector<glm::vec3>& out, const std::vector<float> &raw)
{
    for(int i = 0; i < raw.size(); i+=2)
    {
        out[i / 2].x = raw[i];
        out[i / 2].y = raw[i+1];
    }
}
