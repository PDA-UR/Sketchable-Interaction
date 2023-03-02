

#include "Publisher.hpp"
#include <memory>

Publisher::Publisher():
    Node("Publisher"),
    publisher_(this->create_publisher<std_msgs::msg::String>("/scan_region", 10))
{}

void Publisher::publish(const std::string &message)
{
    auto m = std_msgs::msg::String();
    m.data = message;
    publisher_->publish(m);
}


