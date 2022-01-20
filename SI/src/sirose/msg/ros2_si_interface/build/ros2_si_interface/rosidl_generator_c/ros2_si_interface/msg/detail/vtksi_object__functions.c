// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from ros2_si_interface:msg/VTKSIObject.idl
// generated code does not contain a copyright notice
#include "ros2_si_interface/msg/detail/vtksi_object__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


// Include directives for member types
// Member `plugin`
#include "rosidl_runtime_c/string_functions.h"
// Member `geometry`
// Member `links`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

bool
ros2_si_interface__msg__VTKSIObject__init(ros2_si_interface__msg__VTKSIObject * msg)
{
  if (!msg) {
    return false;
  }
  // id
  // plugin
  if (!rosidl_runtime_c__String__init(&msg->plugin)) {
    ros2_si_interface__msg__VTKSIObject__fini(msg);
    return false;
  }
  // geometry
  if (!rosidl_runtime_c__float__Sequence__init(&msg->geometry, 0)) {
    ros2_si_interface__msg__VTKSIObject__fini(msg);
    return false;
  }
  // click
  // drag
  // x
  // y
  // alive
  // touch
  // links
  if (!rosidl_runtime_c__int32__Sequence__init(&msg->links, 0)) {
    ros2_si_interface__msg__VTKSIObject__fini(msg);
    return false;
  }
  // tracker_dimension_x
  // tracker_dimension_y
  return true;
}

void
ros2_si_interface__msg__VTKSIObject__fini(ros2_si_interface__msg__VTKSIObject * msg)
{
  if (!msg) {
    return;
  }
  // id
  // plugin
  rosidl_runtime_c__String__fini(&msg->plugin);
  // geometry
  rosidl_runtime_c__float__Sequence__fini(&msg->geometry);
  // click
  // drag
  // x
  // y
  // alive
  // touch
  // links
  rosidl_runtime_c__int32__Sequence__fini(&msg->links);
  // tracker_dimension_x
  // tracker_dimension_y
}

ros2_si_interface__msg__VTKSIObject *
ros2_si_interface__msg__VTKSIObject__create()
{
  ros2_si_interface__msg__VTKSIObject * msg = (ros2_si_interface__msg__VTKSIObject *)malloc(sizeof(ros2_si_interface__msg__VTKSIObject));
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(ros2_si_interface__msg__VTKSIObject));
  bool success = ros2_si_interface__msg__VTKSIObject__init(msg);
  if (!success) {
    free(msg);
    return NULL;
  }
  return msg;
}

void
ros2_si_interface__msg__VTKSIObject__destroy(ros2_si_interface__msg__VTKSIObject * msg)
{
  if (msg) {
    ros2_si_interface__msg__VTKSIObject__fini(msg);
  }
  free(msg);
}


bool
ros2_si_interface__msg__VTKSIObject__Sequence__init(ros2_si_interface__msg__VTKSIObject__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  ros2_si_interface__msg__VTKSIObject * data = NULL;
  if (size) {
    data = (ros2_si_interface__msg__VTKSIObject *)calloc(size, sizeof(ros2_si_interface__msg__VTKSIObject));
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = ros2_si_interface__msg__VTKSIObject__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        ros2_si_interface__msg__VTKSIObject__fini(&data[i - 1]);
      }
      free(data);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
ros2_si_interface__msg__VTKSIObject__Sequence__fini(ros2_si_interface__msg__VTKSIObject__Sequence * array)
{
  if (!array) {
    return;
  }
  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      ros2_si_interface__msg__VTKSIObject__fini(&array->data[i]);
    }
    free(array->data);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

ros2_si_interface__msg__VTKSIObject__Sequence *
ros2_si_interface__msg__VTKSIObject__Sequence__create(size_t size)
{
  ros2_si_interface__msg__VTKSIObject__Sequence * array = (ros2_si_interface__msg__VTKSIObject__Sequence *)malloc(sizeof(ros2_si_interface__msg__VTKSIObject__Sequence));
  if (!array) {
    return NULL;
  }
  bool success = ros2_si_interface__msg__VTKSIObject__Sequence__init(array, size);
  if (!success) {
    free(array);
    return NULL;
  }
  return array;
}

void
ros2_si_interface__msg__VTKSIObject__Sequence__destroy(ros2_si_interface__msg__VTKSIObject__Sequence * array)
{
  if (array) {
    ros2_si_interface__msg__VTKSIObject__Sequence__fini(array);
  }
  free(array);
}
