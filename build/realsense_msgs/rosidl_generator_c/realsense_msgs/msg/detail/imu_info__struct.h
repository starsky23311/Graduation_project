// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from realsense_msgs:msg/IMUInfo.idl
// generated code does not contain a copyright notice

#ifndef REALSENSE_MSGS__MSG__DETAIL__IMU_INFO__STRUCT_H_
#define REALSENSE_MSGS__MSG__DETAIL__IMU_INFO__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.h"

// Struct defined in msg/IMUInfo in the package realsense_msgs.
typedef struct realsense_msgs__msg__IMUInfo
{
  std_msgs__msg__Header header;
  double data[12];
  double noise_variances[3];
  double bias_variances[3];
} realsense_msgs__msg__IMUInfo;

// Struct for a sequence of realsense_msgs__msg__IMUInfo.
typedef struct realsense_msgs__msg__IMUInfo__Sequence
{
  realsense_msgs__msg__IMUInfo * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} realsense_msgs__msg__IMUInfo__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // REALSENSE_MSGS__MSG__DETAIL__IMU_INFO__STRUCT_H_
