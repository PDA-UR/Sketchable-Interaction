// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from ros2_si_interface:msg/VTKSIObject.idl
// generated code does not contain a copyright notice

#ifndef ROS2_SI_INTERFACE__MSG__DETAIL__VTKSI_OBJECT__STRUCT_H_
#define ROS2_SI_INTERFACE__MSG__DETAIL__VTKSI_OBJECT__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'plugin'
#include "rosidl_runtime_c/string.h"
// Member 'geometry'
// Member 'links'
#include "rosidl_runtime_c/primitives_sequence.h"

// Struct defined in msg/VTKSIObject in the package ros2_si_interface.
typedef struct ros2_si_interface__msg__VTKSIObject
{
  int64_t id;
  rosidl_runtime_c__String plugin;
  rosidl_runtime_c__float__Sequence geometry;
  bool click;
  bool drag;
  float x;
  float y;
  bool alive;
  bool touch;
  rosidl_runtime_c__int32__Sequence links;
  int32_t tracker_dimension_x;
  int32_t tracker_dimension_y;
} ros2_si_interface__msg__VTKSIObject;

// Struct for a sequence of ros2_si_interface__msg__VTKSIObject.
typedef struct ros2_si_interface__msg__VTKSIObject__Sequence
{
  ros2_si_interface__msg__VTKSIObject * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} ros2_si_interface__msg__VTKSIObject__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // ROS2_SI_INTERFACE__MSG__DETAIL__VTKSI_OBJECT__STRUCT_H_
