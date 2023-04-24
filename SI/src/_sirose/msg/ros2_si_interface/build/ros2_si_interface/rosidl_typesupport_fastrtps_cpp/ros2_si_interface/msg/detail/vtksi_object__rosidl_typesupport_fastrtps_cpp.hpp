// generated from rosidl_typesupport_fastrtps_cpp/resource/idl__rosidl_typesupport_fastrtps_cpp.hpp.em
// with input from ros2_si_interface:msg/VTKSIObject.idl
// generated code does not contain a copyright notice

#ifndef ROS2_SI_INTERFACE__MSG__DETAIL__VTKSI_OBJECT__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_
#define ROS2_SI_INTERFACE__MSG__DETAIL__VTKSI_OBJECT__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_

#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_interface/macros.h"
#include "ros2_si_interface/msg/rosidl_typesupport_fastrtps_cpp__visibility_control.h"
#include "ros2_si_interface/msg/detail/vtksi_object__struct.hpp"

#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-parameter"
# ifdef __clang__
#  pragma clang diagnostic ignored "-Wdeprecated-register"
#  pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
# endif
#endif
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif

#include "fastcdr/Cdr.h"

namespace ros2_si_interface
{

namespace msg
{

namespace typesupport_fastrtps_cpp
{

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_ros2_si_interface
cdr_serialize(
  const ros2_si_interface::msg::VTKSIObject & ros_message,
  eprosima::fastcdr::Cdr & cdr);

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_ros2_si_interface
cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  ros2_si_interface::msg::VTKSIObject & ros_message);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_ros2_si_interface
get_serialized_size(
  const ros2_si_interface::msg::VTKSIObject & ros_message,
  size_t current_alignment);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_ros2_si_interface
max_serialized_size_VTKSIObject(
  bool & full_bounded,
  size_t current_alignment);

}  // namespace typesupport_fastrtps_cpp

}  // namespace msg

}  // namespace ros2_si_interface

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_ros2_si_interface
const rosidl_message_type_support_t *
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, ros2_si_interface, msg, VTKSIObject)();

#ifdef __cplusplus
}
#endif

#endif  // ROS2_SI_INTERFACE__MSG__DETAIL__VTKSI_OBJECT__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_
