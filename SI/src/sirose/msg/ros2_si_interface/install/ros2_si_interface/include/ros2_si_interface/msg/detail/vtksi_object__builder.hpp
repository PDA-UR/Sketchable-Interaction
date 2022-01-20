// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from ros2_si_interface:msg/VTKSIObject.idl
// generated code does not contain a copyright notice

#ifndef ROS2_SI_INTERFACE__MSG__DETAIL__VTKSI_OBJECT__BUILDER_HPP_
#define ROS2_SI_INTERFACE__MSG__DETAIL__VTKSI_OBJECT__BUILDER_HPP_

#include "ros2_si_interface/msg/detail/vtksi_object__struct.hpp"
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <utility>


namespace ros2_si_interface
{

namespace msg
{

namespace builder
{

class Init_VTKSIObject_tracker_dimension_y
{
public:
  explicit Init_VTKSIObject_tracker_dimension_y(::ros2_si_interface::msg::VTKSIObject & msg)
  : msg_(msg)
  {}
  ::ros2_si_interface::msg::VTKSIObject tracker_dimension_y(::ros2_si_interface::msg::VTKSIObject::_tracker_dimension_y_type arg)
  {
    msg_.tracker_dimension_y = std::move(arg);
    return std::move(msg_);
  }

private:
  ::ros2_si_interface::msg::VTKSIObject msg_;
};

class Init_VTKSIObject_tracker_dimension_x
{
public:
  explicit Init_VTKSIObject_tracker_dimension_x(::ros2_si_interface::msg::VTKSIObject & msg)
  : msg_(msg)
  {}
  Init_VTKSIObject_tracker_dimension_y tracker_dimension_x(::ros2_si_interface::msg::VTKSIObject::_tracker_dimension_x_type arg)
  {
    msg_.tracker_dimension_x = std::move(arg);
    return Init_VTKSIObject_tracker_dimension_y(msg_);
  }

private:
  ::ros2_si_interface::msg::VTKSIObject msg_;
};

class Init_VTKSIObject_links
{
public:
  explicit Init_VTKSIObject_links(::ros2_si_interface::msg::VTKSIObject & msg)
  : msg_(msg)
  {}
  Init_VTKSIObject_tracker_dimension_x links(::ros2_si_interface::msg::VTKSIObject::_links_type arg)
  {
    msg_.links = std::move(arg);
    return Init_VTKSIObject_tracker_dimension_x(msg_);
  }

private:
  ::ros2_si_interface::msg::VTKSIObject msg_;
};

class Init_VTKSIObject_touch
{
public:
  explicit Init_VTKSIObject_touch(::ros2_si_interface::msg::VTKSIObject & msg)
  : msg_(msg)
  {}
  Init_VTKSIObject_links touch(::ros2_si_interface::msg::VTKSIObject::_touch_type arg)
  {
    msg_.touch = std::move(arg);
    return Init_VTKSIObject_links(msg_);
  }

private:
  ::ros2_si_interface::msg::VTKSIObject msg_;
};

class Init_VTKSIObject_alive
{
public:
  explicit Init_VTKSIObject_alive(::ros2_si_interface::msg::VTKSIObject & msg)
  : msg_(msg)
  {}
  Init_VTKSIObject_touch alive(::ros2_si_interface::msg::VTKSIObject::_alive_type arg)
  {
    msg_.alive = std::move(arg);
    return Init_VTKSIObject_touch(msg_);
  }

private:
  ::ros2_si_interface::msg::VTKSIObject msg_;
};

class Init_VTKSIObject_y
{
public:
  explicit Init_VTKSIObject_y(::ros2_si_interface::msg::VTKSIObject & msg)
  : msg_(msg)
  {}
  Init_VTKSIObject_alive y(::ros2_si_interface::msg::VTKSIObject::_y_type arg)
  {
    msg_.y = std::move(arg);
    return Init_VTKSIObject_alive(msg_);
  }

private:
  ::ros2_si_interface::msg::VTKSIObject msg_;
};

class Init_VTKSIObject_x
{
public:
  explicit Init_VTKSIObject_x(::ros2_si_interface::msg::VTKSIObject & msg)
  : msg_(msg)
  {}
  Init_VTKSIObject_y x(::ros2_si_interface::msg::VTKSIObject::_x_type arg)
  {
    msg_.x = std::move(arg);
    return Init_VTKSIObject_y(msg_);
  }

private:
  ::ros2_si_interface::msg::VTKSIObject msg_;
};

class Init_VTKSIObject_drag
{
public:
  explicit Init_VTKSIObject_drag(::ros2_si_interface::msg::VTKSIObject & msg)
  : msg_(msg)
  {}
  Init_VTKSIObject_x drag(::ros2_si_interface::msg::VTKSIObject::_drag_type arg)
  {
    msg_.drag = std::move(arg);
    return Init_VTKSIObject_x(msg_);
  }

private:
  ::ros2_si_interface::msg::VTKSIObject msg_;
};

class Init_VTKSIObject_click
{
public:
  explicit Init_VTKSIObject_click(::ros2_si_interface::msg::VTKSIObject & msg)
  : msg_(msg)
  {}
  Init_VTKSIObject_drag click(::ros2_si_interface::msg::VTKSIObject::_click_type arg)
  {
    msg_.click = std::move(arg);
    return Init_VTKSIObject_drag(msg_);
  }

private:
  ::ros2_si_interface::msg::VTKSIObject msg_;
};

class Init_VTKSIObject_geometry
{
public:
  explicit Init_VTKSIObject_geometry(::ros2_si_interface::msg::VTKSIObject & msg)
  : msg_(msg)
  {}
  Init_VTKSIObject_click geometry(::ros2_si_interface::msg::VTKSIObject::_geometry_type arg)
  {
    msg_.geometry = std::move(arg);
    return Init_VTKSIObject_click(msg_);
  }

private:
  ::ros2_si_interface::msg::VTKSIObject msg_;
};

class Init_VTKSIObject_plugin
{
public:
  explicit Init_VTKSIObject_plugin(::ros2_si_interface::msg::VTKSIObject & msg)
  : msg_(msg)
  {}
  Init_VTKSIObject_geometry plugin(::ros2_si_interface::msg::VTKSIObject::_plugin_type arg)
  {
    msg_.plugin = std::move(arg);
    return Init_VTKSIObject_geometry(msg_);
  }

private:
  ::ros2_si_interface::msg::VTKSIObject msg_;
};

class Init_VTKSIObject_id
{
public:
  Init_VTKSIObject_id()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_VTKSIObject_plugin id(::ros2_si_interface::msg::VTKSIObject::_id_type arg)
  {
    msg_.id = std::move(arg);
    return Init_VTKSIObject_plugin(msg_);
  }

private:
  ::ros2_si_interface::msg::VTKSIObject msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::ros2_si_interface::msg::VTKSIObject>()
{
  return ros2_si_interface::msg::builder::Init_VTKSIObject_id();
}

}  // namespace ros2_si_interface

#endif  // ROS2_SI_INTERFACE__MSG__DETAIL__VTKSI_OBJECT__BUILDER_HPP_
