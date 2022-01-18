

#ifndef SITEST_SUBSCRIBER_HPP
#define SITEST_SUBSCRIBER_HPP

#include <SI/SI.hpp>
#include <rclcpp/rclcpp.hpp>

#include <ros2_si_interface/msg/vtksi_object.hpp>

class Subscriber: public rclcpp::Node, public SIObject
{ SIROSE
public:
    Subscriber();

private:
    rclcpp::SubscriptionBase::SharedPtr sp_subscription;
    void object_callback(ros2_si_interface::msg::VTKSIObject::SharedPtr message);

    void to_shape(std::vector<glm::vec3>& out, const std::vector<float>& raw);
};



#endif //SITEST_SUBSCRIBER_HPP
