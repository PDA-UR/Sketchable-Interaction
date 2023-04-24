// generated from rosidl_typesupport_fastrtps_cpp/resource/idl__type_support.cpp.em
// with input from ros2_si_interface:msg/VTKSIObject.idl
// generated code does not contain a copyright notice
#include "ros2_si_interface/msg/detail/vtksi_object__rosidl_typesupport_fastrtps_cpp.hpp"
#include "ros2_si_interface/msg/detail/vtksi_object__struct.hpp"

#include <limits>
#include <stdexcept>
#include <string>
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_fastrtps_cpp/identifier.hpp"
#include "rosidl_typesupport_fastrtps_cpp/message_type_support.h"
#include "rosidl_typesupport_fastrtps_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_fastrtps_cpp/wstring_conversion.hpp"
#include "fastcdr/Cdr.h"


// forward declaration of message dependencies and their conversion functions

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
  eprosima::fastcdr::Cdr & cdr)
{
  // Member: id
  cdr << ros_message.id;
  // Member: plugin
  cdr << ros_message.plugin;
  // Member: geometry
  {
    cdr << ros_message.geometry;
  }
  // Member: click
  cdr << (ros_message.click ? true : false);
  // Member: drag
  cdr << (ros_message.drag ? true : false);
  // Member: dblclick
  cdr << (ros_message.dblclick ? true : false);
  // Member: x
  cdr << ros_message.x;
  // Member: y
  cdr << ros_message.y;
  // Member: alive
  cdr << (ros_message.alive ? true : false);
  // Member: touch
  cdr << (ros_message.touch ? true : false);
  // Member: links
  {
    cdr << ros_message.links;
  }
  // Member: tracker_dimension_x
  cdr << ros_message.tracker_dimension_x;
  // Member: tracker_dimension_y
  cdr << ros_message.tracker_dimension_y;
  // Member: color
  {
    cdr << ros_message.color;
  }
  return true;
}

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_ros2_si_interface
cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  ros2_si_interface::msg::VTKSIObject & ros_message)
{
  // Member: id
  cdr >> ros_message.id;

  // Member: plugin
  cdr >> ros_message.plugin;

  // Member: geometry
  {
    cdr >> ros_message.geometry;
  }

  // Member: click
  {
    uint8_t tmp;
    cdr >> tmp;
    ros_message.click = tmp ? true : false;
  }

  // Member: drag
  {
    uint8_t tmp;
    cdr >> tmp;
    ros_message.drag = tmp ? true : false;
  }

  // Member: dblclick
  {
    uint8_t tmp;
    cdr >> tmp;
    ros_message.dblclick = tmp ? true : false;
  }

  // Member: x
  cdr >> ros_message.x;

  // Member: y
  cdr >> ros_message.y;

  // Member: alive
  {
    uint8_t tmp;
    cdr >> tmp;
    ros_message.alive = tmp ? true : false;
  }

  // Member: touch
  {
    uint8_t tmp;
    cdr >> tmp;
    ros_message.touch = tmp ? true : false;
  }

  // Member: links
  {
    cdr >> ros_message.links;
  }

  // Member: tracker_dimension_x
  cdr >> ros_message.tracker_dimension_x;

  // Member: tracker_dimension_y
  cdr >> ros_message.tracker_dimension_y;

  // Member: color
  {
    cdr >> ros_message.color;
  }

  return true;
}

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_ros2_si_interface
get_serialized_size(
  const ros2_si_interface::msg::VTKSIObject & ros_message,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // Member: id
  {
    size_t item_size = sizeof(ros_message.id);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: plugin
  current_alignment += padding +
    eprosima::fastcdr::Cdr::alignment(current_alignment, padding) +
    (ros_message.plugin.size() + 1);
  // Member: geometry
  {
    size_t array_size = ros_message.geometry.size();

    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    size_t item_size = sizeof(ros_message.geometry[0]);
    current_alignment += array_size * item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: click
  {
    size_t item_size = sizeof(ros_message.click);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: drag
  {
    size_t item_size = sizeof(ros_message.drag);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: dblclick
  {
    size_t item_size = sizeof(ros_message.dblclick);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: x
  {
    size_t item_size = sizeof(ros_message.x);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: y
  {
    size_t item_size = sizeof(ros_message.y);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: alive
  {
    size_t item_size = sizeof(ros_message.alive);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: touch
  {
    size_t item_size = sizeof(ros_message.touch);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: links
  {
    size_t array_size = ros_message.links.size();

    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    size_t item_size = sizeof(ros_message.links[0]);
    current_alignment += array_size * item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: tracker_dimension_x
  {
    size_t item_size = sizeof(ros_message.tracker_dimension_x);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: tracker_dimension_y
  {
    size_t item_size = sizeof(ros_message.tracker_dimension_y);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: color
  {
    size_t array_size = ros_message.color.size();

    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    size_t item_size = sizeof(ros_message.color[0]);
    current_alignment += array_size * item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_ros2_si_interface
max_serialized_size_VTKSIObject(
  bool & full_bounded,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;
  (void)full_bounded;


  // Member: id
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint64_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint64_t));
  }

  // Member: plugin
  {
    size_t array_size = 1;

    full_bounded = false;
    for (size_t index = 0; index < array_size; ++index) {
      current_alignment += padding +
        eprosima::fastcdr::Cdr::alignment(current_alignment, padding) +
        1;
    }
  }

  // Member: geometry
  {
    size_t array_size = 0;
    full_bounded = false;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: click
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint8_t);
  }

  // Member: drag
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint8_t);
  }

  // Member: dblclick
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint8_t);
  }

  // Member: x
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: y
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: alive
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint8_t);
  }

  // Member: touch
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint8_t);
  }

  // Member: links
  {
    size_t array_size = 0;
    full_bounded = false;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: tracker_dimension_x
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: tracker_dimension_y
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: color
  {
    size_t array_size = 0;
    full_bounded = false;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  return current_alignment - initial_alignment;
}

static bool _VTKSIObject__cdr_serialize(
  const void * untyped_ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  auto typed_message =
    static_cast<const ros2_si_interface::msg::VTKSIObject *>(
    untyped_ros_message);
  return cdr_serialize(*typed_message, cdr);
}

static bool _VTKSIObject__cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  void * untyped_ros_message)
{
  auto typed_message =
    static_cast<ros2_si_interface::msg::VTKSIObject *>(
    untyped_ros_message);
  return cdr_deserialize(cdr, *typed_message);
}

static uint32_t _VTKSIObject__get_serialized_size(
  const void * untyped_ros_message)
{
  auto typed_message =
    static_cast<const ros2_si_interface::msg::VTKSIObject *>(
    untyped_ros_message);
  return static_cast<uint32_t>(get_serialized_size(*typed_message, 0));
}

static size_t _VTKSIObject__max_serialized_size(bool & full_bounded)
{
  return max_serialized_size_VTKSIObject(full_bounded, 0);
}

static message_type_support_callbacks_t _VTKSIObject__callbacks = {
  "ros2_si_interface::msg",
  "VTKSIObject",
  _VTKSIObject__cdr_serialize,
  _VTKSIObject__cdr_deserialize,
  _VTKSIObject__get_serialized_size,
  _VTKSIObject__max_serialized_size
};

static rosidl_message_type_support_t _VTKSIObject__handle = {
  rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
  &_VTKSIObject__callbacks,
  get_message_typesupport_handle_function,
};

}  // namespace typesupport_fastrtps_cpp

}  // namespace msg

}  // namespace ros2_si_interface

namespace rosidl_typesupport_fastrtps_cpp
{

template<>
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_EXPORT_ros2_si_interface
const rosidl_message_type_support_t *
get_message_type_support_handle<ros2_si_interface::msg::VTKSIObject>()
{
  return &ros2_si_interface::msg::typesupport_fastrtps_cpp::_VTKSIObject__handle;
}

}  // namespace rosidl_typesupport_fastrtps_cpp

#ifdef __cplusplus
extern "C"
{
#endif

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, ros2_si_interface, msg, VTKSIObject)() {
  return &ros2_si_interface::msg::typesupport_fastrtps_cpp::_VTKSIObject__handle;
}

#ifdef __cplusplus
}
#endif
