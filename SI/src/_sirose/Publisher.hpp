

#ifndef SITEST_PUBLISHER_HPP
#define SITEST_PUBLISHER_HPP

#include <SI/SI.hpp>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

class Publisher: public rclcpp::Node, public SIObject
{ SIROSE
public:
    Publisher();
    void publish(const std::string& message);

private:
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
};


#endif //SITEST_PUBLISHER_HPP
