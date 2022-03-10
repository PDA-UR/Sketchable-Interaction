// generated from rosidl_typesupport_fastrtps_c/resource/idl__type_support_c.cpp.em
// with input from ros2_si_interface:msg/VTKSIObject.idl
// generated code does not contain a copyright notice
#include "ros2_si_interface/msg/detail/vtksi_object__rosidl_typesupport_fastrtps_c.h"


#include <cassert>
#include <limits>
#include <string>
#include "rosidl_typesupport_fastrtps_c/identifier.h"
#include "rosidl_typesupport_fastrtps_c/wstring_conversion.hpp"
#include "rosidl_typesupport_fastrtps_cpp/message_type_support.h"
#include "ros2_si_interface/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
#include "ros2_si_interface/msg/detail/vtksi_object__struct.h"
#include "ros2_si_interface/msg/detail/vtksi_object__functions.h"
#include "fastcdr/Cdr.h"

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

// includes and forward declarations of message dependencies and their conversion functions

#if defined(__cplusplus)
extern "C"
{
#endif

#include "rosidl_runtime_c/primitives_sequence.h"  // color, geometry, links
#include "rosidl_runtime_c/primitives_sequence_functions.h"  // color, geometry, links
#include "rosidl_runtime_c/string.h"  // plugin
#include "rosidl_runtime_c/string_functions.h"  // plugin

// forward declare type support functions


using _VTKSIObject__ros_msg_type = ros2_si_interface__msg__VTKSIObject;

static bool _VTKSIObject__cdr_serialize(
  const void * untyped_ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  const _VTKSIObject__ros_msg_type * ros_message = static_cast<const _VTKSIObject__ros_msg_type *>(untyped_ros_message);
  // Field name: id
  {
    cdr << ros_message->id;
  }

  // Field name: plugin
  {
    const rosidl_runtime_c__String * str = &ros_message->plugin;
    if (str->capacity == 0 || str->capacity <= str->size) {
      fprintf(stderr, "string capacity not greater than size\n");
      return false;
    }
    if (str->data[str->size] != '\0') {
      fprintf(stderr, "string not null-terminated\n");
      return false;
    }
    cdr << str->data;
  }

  // Field name: geometry
  {
    size_t size = ros_message->geometry.size;
    auto array_ptr = ros_message->geometry.data;
    cdr << static_cast<uint32_t>(size);
    cdr.serializeArray(array_ptr, size);
  }

  // Field name: click
  {
    cdr << (ros_message->click ? true : false);
  }

  // Field name: drag
  {
    cdr << (ros_message->drag ? true : false);
  }

  // Field name: x
  {
    cdr << ros_message->x;
  }

  // Field name: y
  {
    cdr << ros_message->y;
  }

  // Field name: alive
  {
    cdr << (ros_message->alive ? true : false);
  }

  // Field name: touch
  {
    cdr << (ros_message->touch ? true : false);
  }

  // Field name: links
  {
    size_t size = ros_message->links.size;
    auto array_ptr = ros_message->links.data;
    cdr << static_cast<uint32_t>(size);
    cdr.serializeArray(array_ptr, size);
  }

  // Field name: tracker_dimension_x
  {
    cdr << ros_message->tracker_dimension_x;
  }

  // Field name: tracker_dimension_y
  {
    cdr << ros_message->tracker_dimension_y;
  }

  // Field name: color
  {
    size_t size = ros_message->color.size;
    auto array_ptr = ros_message->color.data;
    cdr << static_cast<uint32_t>(size);
    cdr.serializeArray(array_ptr, size);
  }

  return true;
}

static bool _VTKSIObject__cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  void * untyped_ros_message)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  _VTKSIObject__ros_msg_type * ros_message = static_cast<_VTKSIObject__ros_msg_type *>(untyped_ros_message);
  // Field name: id
  {
    cdr >> ros_message->id;
  }

  // Field name: plugin
  {
    std::string tmp;
    cdr >> tmp;
    if (!ros_message->plugin.data) {
      rosidl_runtime_c__String__init(&ros_message->plugin);
    }
    bool succeeded = rosidl_runtime_c__String__assign(
      &ros_message->plugin,
      tmp.c_str());
    if (!succeeded) {
      fprintf(stderr, "failed to assign string into field 'plugin'\n");
      return false;
    }
  }

  // Field name: geometry
  {
    uint32_t cdrSize;
    cdr >> cdrSize;
    size_t size = static_cast<size_t>(cdrSize);
    if (ros_message->geometry.data) {
      rosidl_runtime_c__float__Sequence__fini(&ros_message->geometry);
    }
    if (!rosidl_runtime_c__float__Sequence__init(&ros_message->geometry, size)) {
      fprintf(stderr, "failed to create array for field 'geometry'");
      return false;
    }
    auto array_ptr = ros_message->geometry.data;
    cdr.deserializeArray(array_ptr, size);
  }

  // Field name: click
  {
    uint8_t tmp;
    cdr >> tmp;
    ros_message->click = tmp ? true : false;
  }

  // Field name: drag
  {
    uint8_t tmp;
    cdr >> tmp;
    ros_message->drag = tmp ? true : false;
  }

  // Field name: x
  {
    cdr >> ros_message->x;
  }

  // Field name: y
  {
    cdr >> ros_message->y;
  }

  // Field name: alive
  {
    uint8_t tmp;
    cdr >> tmp;
    ros_message->alive = tmp ? true : false;
  }

  // Field name: touch
  {
    uint8_t tmp;
    cdr >> tmp;
    ros_message->touch = tmp ? true : false;
  }

  // Field name: links
  {
    uint32_t cdrSize;
    cdr >> cdrSize;
    size_t size = static_cast<size_t>(cdrSize);
    if (ros_message->links.data) {
      rosidl_runtime_c__int32__Sequence__fini(&ros_message->links);
    }
    if (!rosidl_runtime_c__int32__Sequence__init(&ros_message->links, size)) {
      fprintf(stderr, "failed to create array for field 'links'");
      return false;
    }
    auto array_ptr = ros_message->links.data;
    cdr.deserializeArray(array_ptr, size);
  }

  // Field name: tracker_dimension_x
  {
    cdr >> ros_message->tracker_dimension_x;
  }

  // Field name: tracker_dimension_y
  {
    cdr >> ros_message->tracker_dimension_y;
  }

  // Field name: color
  {
    uint32_t cdrSize;
    cdr >> cdrSize;
    size_t size = static_cast<size_t>(cdrSize);
    if (ros_message->color.data) {
      rosidl_runtime_c__int32__Sequence__fini(&ros_message->color);
    }
    if (!rosidl_runtime_c__int32__Sequence__init(&ros_message->color, size)) {
      fprintf(stderr, "failed to create array for field 'color'");
      return false;
    }
    auto array_ptr = ros_message->color.data;
    cdr.deserializeArray(array_ptr, size);
  }

  return true;
}  // NOLINT(readability/fn_size)

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_ros2_si_interface
size_t get_serialized_size_ros2_si_interface__msg__VTKSIObject(
  const void * untyped_ros_message,
  size_t current_alignment)
{
  const _VTKSIObject__ros_msg_type * ros_message = static_cast<const _VTKSIObject__ros_msg_type *>(untyped_ros_message);
  (void)ros_message;
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // field.name id
  {
    size_t item_size = sizeof(ros_message->id);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name plugin
  current_alignment += padding +
    eprosima::fastcdr::Cdr::alignment(current_alignment, padding) +
    (ros_message->plugin.size + 1);
  // field.name geometry
  {
    size_t array_size = ros_message->geometry.size;
    auto array_ptr = ros_message->geometry.data;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    (void)array_ptr;
    size_t item_size = sizeof(array_ptr[0]);
    current_alignment += array_size * item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name click
  {
    size_t item_size = sizeof(ros_message->click);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name drag
  {
    size_t item_size = sizeof(ros_message->drag);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name x
  {
    size_t item_size = sizeof(ros_message->x);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name y
  {
    size_t item_size = sizeof(ros_message->y);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name alive
  {
    size_t item_size = sizeof(ros_message->alive);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name touch
  {
    size_t item_size = sizeof(ros_message->touch);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name links
  {
    size_t array_size = ros_message->links.size;
    auto array_ptr = ros_message->links.data;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    (void)array_ptr;
    size_t item_size = sizeof(array_ptr[0]);
    current_alignment += array_size * item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name tracker_dimension_x
  {
    size_t item_size = sizeof(ros_message->tracker_dimension_x);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name tracker_dimension_y
  {
    size_t item_size = sizeof(ros_message->tracker_dimension_y);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name color
  {
    size_t array_size = ros_message->color.size;
    auto array_ptr = ros_message->color.data;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    (void)array_ptr;
    size_t item_size = sizeof(array_ptr[0]);
    current_alignment += array_size * item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}

static uint32_t _VTKSIObject__get_serialized_size(const void * untyped_ros_message)
{
  return static_cast<uint32_t>(
    get_serialized_size_ros2_si_interface__msg__VTKSIObject(
      untyped_ros_message, 0));
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_ros2_si_interface
size_t max_serialized_size_ros2_si_interface__msg__VTKSIObject(
  bool & full_bounded,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;
  (void)full_bounded;

  // member: id
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint64_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint64_t));
  }
  // member: plugin
  {
    size_t array_size = 1;

    full_bounded = false;
    for (size_t index = 0; index < array_size; ++index) {
      current_alignment += padding +
        eprosima::fastcdr::Cdr::alignment(current_alignment, padding) +
        1;
    }
  }
  // member: geometry
  {
    size_t array_size = 0;
    full_bounded = false;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: click
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint8_t);
  }
  // member: drag
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint8_t);
  }
  // member: x
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: y
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: alive
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint8_t);
  }
  // member: touch
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint8_t);
  }
  // member: links
  {
    size_t array_size = 0;
    full_bounded = false;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: tracker_dimension_x
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: tracker_dimension_y
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: color
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

static size_t _VTKSIObject__max_serialized_size(bool & full_bounded)
{
  return max_serialized_size_ros2_si_interface__msg__VTKSIObject(
    full_bounded, 0);
}


static message_type_support_callbacks_t __callbacks_VTKSIObject = {
  "ros2_si_interface::msg",
  "VTKSIObject",
  _VTKSIObject__cdr_serialize,
  _VTKSIObject__cdr_deserialize,
  _VTKSIObject__get_serialized_size,
  _VTKSIObject__max_serialized_size
};

static rosidl_message_type_support_t _VTKSIObject__type_support = {
  rosidl_typesupport_fastrtps_c__identifier,
  &__callbacks_VTKSIObject,
  get_message_typesupport_handle_function,
};

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, ros2_si_interface, msg, VTKSIObject)() {
  return &_VTKSIObject__type_support;
}

#if defined(__cplusplus)
}
#endif
