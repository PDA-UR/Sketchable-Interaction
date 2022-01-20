// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from ros2_si_interface:msg/VTKSIObject.idl
// generated code does not contain a copyright notice

#ifndef ROS2_SI_INTERFACE__MSG__DETAIL__VTKSI_OBJECT__TRAITS_HPP_
#define ROS2_SI_INTERFACE__MSG__DETAIL__VTKSI_OBJECT__TRAITS_HPP_

#include "ros2_si_interface/msg/detail/vtksi_object__struct.hpp"
#include <stdint.h>
#include <rosidl_runtime_cpp/traits.hpp>
#include <sstream>
#include <string>
#include <type_traits>

namespace rosidl_generator_traits
{

inline void to_yaml(
  const ros2_si_interface::msg::VTKSIObject & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: id
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "id: ";
    value_to_yaml(msg.id, out);
    out << "\n";
  }

  // member: plugin
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "plugin: ";
    value_to_yaml(msg.plugin, out);
    out << "\n";
  }

  // member: geometry
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.geometry.size() == 0) {
      out << "geometry: []\n";
    } else {
      out << "geometry:\n";
      for (auto item : msg.geometry) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: click
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "click: ";
    value_to_yaml(msg.click, out);
    out << "\n";
  }

  // member: drag
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "drag: ";
    value_to_yaml(msg.drag, out);
    out << "\n";
  }

  // member: x
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "x: ";
    value_to_yaml(msg.x, out);
    out << "\n";
  }

  // member: y
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "y: ";
    value_to_yaml(msg.y, out);
    out << "\n";
  }

  // member: alive
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "alive: ";
    value_to_yaml(msg.alive, out);
    out << "\n";
  }

  // member: touch
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "touch: ";
    value_to_yaml(msg.touch, out);
    out << "\n";
  }

  // member: links
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.links.size() == 0) {
      out << "links: []\n";
    } else {
      out << "links:\n";
      for (auto item : msg.links) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: tracker_dimension_x
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "tracker_dimension_x: ";
    value_to_yaml(msg.tracker_dimension_x, out);
    out << "\n";
  }

  // member: tracker_dimension_y
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "tracker_dimension_y: ";
    value_to_yaml(msg.tracker_dimension_y, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const ros2_si_interface::msg::VTKSIObject & msg)
{
  std::ostringstream out;
  to_yaml(msg, out);
  return out.str();
}

template<>
inline const char * data_type<ros2_si_interface::msg::VTKSIObject>()
{
  return "ros2_si_interface::msg::VTKSIObject";
}

template<>
inline const char * name<ros2_si_interface::msg::VTKSIObject>()
{
  return "ros2_si_interface/msg/VTKSIObject";
}

template<>
struct has_fixed_size<ros2_si_interface::msg::VTKSIObject>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<ros2_si_interface::msg::VTKSIObject>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<ros2_si_interface::msg::VTKSIObject>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // ROS2_SI_INTERFACE__MSG__DETAIL__VTKSI_OBJECT__TRAITS_HPP_
