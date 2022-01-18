#include "SIROS2Environment.hpp"
#include <rclcpp/rclcpp.hpp>

SIROS2Environment::SIROS2Environment() = default;
SIROS2Environment::~SIROS2Environment() = default;

void SIROS2Environment::start(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    sub = std::make_shared<Subscriber>();
    std::thread{[this]()
    {
        rclcpp::spin(sub);
    }}.detach();
}

void SIROS2Environment::stop()
{
    rclcpp::shutdown();
}
