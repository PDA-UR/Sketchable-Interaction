// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from ros2_si_interface:msg/VTKSIObject.idl
// generated code does not contain a copyright notice

#ifndef ROS2_SI_INTERFACE__MSG__DETAIL__VTKSI_OBJECT__STRUCT_HPP_
#define ROS2_SI_INTERFACE__MSG__DETAIL__VTKSI_OBJECT__STRUCT_HPP_

#include <rosidl_runtime_cpp/bounded_vector.hpp>
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>


#ifndef _WIN32
# define DEPRECATED__ros2_si_interface__msg__VTKSIObject __attribute__((deprecated))
#else
# define DEPRECATED__ros2_si_interface__msg__VTKSIObject __declspec(deprecated)
#endif

namespace ros2_si_interface
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct VTKSIObject_
{
  using Type = VTKSIObject_<ContainerAllocator>;

  explicit VTKSIObject_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::DEFAULTS_ONLY == _init)
    {
      this->color.resize(3);
      this->color = {{0l, 255l, 0l}};
    } else if (rosidl_runtime_cpp::MessageInitialization::ZERO == _init) {
      this->id = 0ll;
      this->plugin = "";
      this->click = false;
      this->drag = false;
      this->x = 0.0f;
      this->y = 0.0f;
      this->alive = false;
      this->touch = false;
      this->tracker_dimension_x = 0l;
      this->tracker_dimension_y = 0l;
    }
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->id = 0ll;
      this->plugin = "";
      this->click = false;
      this->drag = false;
      this->x = 0.0f;
      this->y = 0.0f;
      this->alive = false;
      this->touch = false;
      this->tracker_dimension_x = 0l;
      this->tracker_dimension_y = 0l;
    }
  }

  explicit VTKSIObject_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : plugin(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::DEFAULTS_ONLY == _init)
    {
      this->color.resize(3);
      this->color = {{0l, 255l, 0l}};
    } else if (rosidl_runtime_cpp::MessageInitialization::ZERO == _init) {
      this->id = 0ll;
      this->plugin = "";
      this->click = false;
      this->drag = false;
      this->x = 0.0f;
      this->y = 0.0f;
      this->alive = false;
      this->touch = false;
      this->tracker_dimension_x = 0l;
      this->tracker_dimension_y = 0l;
    }
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->id = 0ll;
      this->plugin = "";
      this->click = false;
      this->drag = false;
      this->x = 0.0f;
      this->y = 0.0f;
      this->alive = false;
      this->touch = false;
      this->tracker_dimension_x = 0l;
      this->tracker_dimension_y = 0l;
    }
  }

  // field types and members
  using _id_type =
    int64_t;
  _id_type id;
  using _plugin_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _plugin_type plugin;
  using _geometry_type =
    std::vector<float, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<float>>;
  _geometry_type geometry;
  using _click_type =
    bool;
  _click_type click;
  using _drag_type =
    bool;
  _drag_type drag;
  using _x_type =
    float;
  _x_type x;
  using _y_type =
    float;
  _y_type y;
  using _alive_type =
    bool;
  _alive_type alive;
  using _touch_type =
    bool;
  _touch_type touch;
  using _links_type =
    std::vector<int32_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<int32_t>>;
  _links_type links;
  using _tracker_dimension_x_type =
    int32_t;
  _tracker_dimension_x_type tracker_dimension_x;
  using _tracker_dimension_y_type =
    int32_t;
  _tracker_dimension_y_type tracker_dimension_y;
  using _color_type =
    std::vector<int32_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<int32_t>>;
  _color_type color;

  // setters for named parameter idiom
  Type & set__id(
    const int64_t & _arg)
  {
    this->id = _arg;
    return *this;
  }
  Type & set__plugin(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->plugin = _arg;
    return *this;
  }
  Type & set__geometry(
    const std::vector<float, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<float>> & _arg)
  {
    this->geometry = _arg;
    return *this;
  }
  Type & set__click(
    const bool & _arg)
  {
    this->click = _arg;
    return *this;
  }
  Type & set__drag(
    const bool & _arg)
  {
    this->drag = _arg;
    return *this;
  }
  Type & set__x(
    const float & _arg)
  {
    this->x = _arg;
    return *this;
  }
  Type & set__y(
    const float & _arg)
  {
    this->y = _arg;
    return *this;
  }
  Type & set__alive(
    const bool & _arg)
  {
    this->alive = _arg;
    return *this;
  }
  Type & set__touch(
    const bool & _arg)
  {
    this->touch = _arg;
    return *this;
  }
  Type & set__links(
    const std::vector<int32_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<int32_t>> & _arg)
  {
    this->links = _arg;
    return *this;
  }
  Type & set__tracker_dimension_x(
    const int32_t & _arg)
  {
    this->tracker_dimension_x = _arg;
    return *this;
  }
  Type & set__tracker_dimension_y(
    const int32_t & _arg)
  {
    this->tracker_dimension_y = _arg;
    return *this;
  }
  Type & set__color(
    const std::vector<int32_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<int32_t>> & _arg)
  {
    this->color = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    ros2_si_interface::msg::VTKSIObject_<ContainerAllocator> *;
  using ConstRawPtr =
    const ros2_si_interface::msg::VTKSIObject_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<ros2_si_interface::msg::VTKSIObject_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<ros2_si_interface::msg::VTKSIObject_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      ros2_si_interface::msg::VTKSIObject_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<ros2_si_interface::msg::VTKSIObject_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      ros2_si_interface::msg::VTKSIObject_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<ros2_si_interface::msg::VTKSIObject_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<ros2_si_interface::msg::VTKSIObject_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<ros2_si_interface::msg::VTKSIObject_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__ros2_si_interface__msg__VTKSIObject
    std::shared_ptr<ros2_si_interface::msg::VTKSIObject_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__ros2_si_interface__msg__VTKSIObject
    std::shared_ptr<ros2_si_interface::msg::VTKSIObject_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const VTKSIObject_ & other) const
  {
    if (this->id != other.id) {
      return false;
    }
    if (this->plugin != other.plugin) {
      return false;
    }
    if (this->geometry != other.geometry) {
      return false;
    }
    if (this->click != other.click) {
      return false;
    }
    if (this->drag != other.drag) {
      return false;
    }
    if (this->x != other.x) {
      return false;
    }
    if (this->y != other.y) {
      return false;
    }
    if (this->alive != other.alive) {
      return false;
    }
    if (this->touch != other.touch) {
      return false;
    }
    if (this->links != other.links) {
      return false;
    }
    if (this->tracker_dimension_x != other.tracker_dimension_x) {
      return false;
    }
    if (this->tracker_dimension_y != other.tracker_dimension_y) {
      return false;
    }
    if (this->color != other.color) {
      return false;
    }
    return true;
  }
  bool operator!=(const VTKSIObject_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct VTKSIObject_

// alias to use template instance with default allocator
using VTKSIObject =
  ros2_si_interface::msg::VTKSIObject_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace ros2_si_interface

#endif  // ROS2_SI_INTERFACE__MSG__DETAIL__VTKSI_OBJECT__STRUCT_HPP_
