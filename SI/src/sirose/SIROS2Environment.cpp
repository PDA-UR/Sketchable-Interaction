#include "SIROS2Environment.hpp"
#include <rclcpp/rclcpp.hpp>
#include <debug/Print.hpp>

SIROS2Environment::SIROS2Environment() = default;
SIROS2Environment::~SIROS2Environment() = default;

void SIROS2Environment::start(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    sub = std::make_shared<Subscriber>();
    pub = std::make_shared<Publisher>();

    std::thread{[this]()
    {
        rclcpp::spin(sub);
    }}.detach();

    std::thread{[this]()
    {
        rclcpp::spin(pub);
    }}.detach();
}

void SIROS2Environment::stop()
{
    rclcpp::shutdown();
}

void SIROS2Environment::send(const std::string &msg) const
{
    pub->publish(msg);
}
