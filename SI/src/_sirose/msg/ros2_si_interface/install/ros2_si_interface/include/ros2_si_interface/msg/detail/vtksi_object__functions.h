// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from ros2_si_interface:msg/VTKSIObject.idl
// generated code does not contain a copyright notice

#ifndef ROS2_SI_INTERFACE__MSG__DETAIL__VTKSI_OBJECT__FUNCTIONS_H_
#define ROS2_SI_INTERFACE__MSG__DETAIL__VTKSI_OBJECT__FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "rosidl_runtime_c/visibility_control.h"
#include "ros2_si_interface/msg/rosidl_generator_c__visibility_control.h"

#include "ros2_si_interface/msg/detail/vtksi_object__struct.h"

/// Initialize msg/VTKSIObject message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * ros2_si_interface__msg__VTKSIObject
 * )) before or use
 * ros2_si_interface__msg__VTKSIObject__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_ros2_si_interface
bool
ros2_si_interface__msg__VTKSIObject__init(ros2_si_interface__msg__VTKSIObject * msg);

/// Finalize msg/VTKSIObject message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_ros2_si_interface
void
ros2_si_interface__msg__VTKSIObject__fini(ros2_si_interface__msg__VTKSIObject * msg);

/// Create msg/VTKSIObject message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * ros2_si_interface__msg__VTKSIObject__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_ros2_si_interface
ros2_si_interface__msg__VTKSIObject *
ros2_si_interface__msg__VTKSIObject__create();

/// Destroy msg/VTKSIObject message.
/**
 * It calls
 * ros2_si_interface__msg__VTKSIObject__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_ros2_si_interface
void
ros2_si_interface__msg__VTKSIObject__destroy(ros2_si_interface__msg__VTKSIObject * msg);


/// Initialize array of msg/VTKSIObject messages.
/**
 * It allocates the memory for the number of elements and calls
 * ros2_si_interface__msg__VTKSIObject__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_ros2_si_interface
bool
ros2_si_interface__msg__VTKSIObject__Sequence__init(ros2_si_interface__msg__VTKSIObject__Sequence * array, size_t size);

/// Finalize array of msg/VTKSIObject messages.
/**
 * It calls
 * ros2_si_interface__msg__VTKSIObject__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_ros2_si_interface
void
ros2_si_interface__msg__VTKSIObject__Sequence__fini(ros2_si_interface__msg__VTKSIObject__Sequence * array);

/// Create array of msg/VTKSIObject messages.
/**
 * It allocates the memory for the array and calls
 * ros2_si_interface__msg__VTKSIObject__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_ros2_si_interface
ros2_si_interface__msg__VTKSIObject__Sequence *
ros2_si_interface__msg__VTKSIObject__Sequence__create(size_t size);

/// Destroy array of msg/VTKSIObject messages.
/**
 * It calls
 * ros2_si_interface__msg__VTKSIObject__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_ros2_si_interface
void
ros2_si_interface__msg__VTKSIObject__Sequence__destroy(ros2_si_interface__msg__VTKSIObject__Sequence * array);

#ifdef __cplusplus
}
#endif

#endif  // ROS2_SI_INTERFACE__MSG__DETAIL__VTKSI_OBJECT__FUNCTIONS_H_
