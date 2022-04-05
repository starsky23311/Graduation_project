// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from realsense_msgs:msg/IMUInfo.idl
// generated code does not contain a copyright notice
#include "realsense_msgs/msg/detail/imu_info__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


// Include directives for member types
// Member `header`
#include "std_msgs/msg/detail/header__functions.h"

bool
realsense_msgs__msg__IMUInfo__init(realsense_msgs__msg__IMUInfo * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__init(&msg->header)) {
    realsense_msgs__msg__IMUInfo__fini(msg);
    return false;
  }
  // data
  // noise_variances
  // bias_variances
  return true;
}

void
realsense_msgs__msg__IMUInfo__fini(realsense_msgs__msg__IMUInfo * msg)
{
  if (!msg) {
    return;
  }
  // header
  std_msgs__msg__Header__fini(&msg->header);
  // data
  // noise_variances
  // bias_variances
}

realsense_msgs__msg__IMUInfo *
realsense_msgs__msg__IMUInfo__create()
{
  realsense_msgs__msg__IMUInfo * msg = (realsense_msgs__msg__IMUInfo *)malloc(sizeof(realsense_msgs__msg__IMUInfo));
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(realsense_msgs__msg__IMUInfo));
  bool success = realsense_msgs__msg__IMUInfo__init(msg);
  if (!success) {
    free(msg);
    return NULL;
  }
  return msg;
}

void
realsense_msgs__msg__IMUInfo__destroy(realsense_msgs__msg__IMUInfo * msg)
{
  if (msg) {
    realsense_msgs__msg__IMUInfo__fini(msg);
  }
  free(msg);
}


bool
realsense_msgs__msg__IMUInfo__Sequence__init(realsense_msgs__msg__IMUInfo__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  realsense_msgs__msg__IMUInfo * data = NULL;
  if (size) {
    data = (realsense_msgs__msg__IMUInfo *)calloc(size, sizeof(realsense_msgs__msg__IMUInfo));
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = realsense_msgs__msg__IMUInfo__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        realsense_msgs__msg__IMUInfo__fini(&data[i - 1]);
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
realsense_msgs__msg__IMUInfo__Sequence__fini(realsense_msgs__msg__IMUInfo__Sequence * array)
{
  if (!array) {
    return;
  }
  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      realsense_msgs__msg__IMUInfo__fini(&array->data[i]);
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

realsense_msgs__msg__IMUInfo__Sequence *
realsense_msgs__msg__IMUInfo__Sequence__create(size_t size)
{
  realsense_msgs__msg__IMUInfo__Sequence * array = (realsense_msgs__msg__IMUInfo__Sequence *)malloc(sizeof(realsense_msgs__msg__IMUInfo__Sequence));
  if (!array) {
    return NULL;
  }
  bool success = realsense_msgs__msg__IMUInfo__Sequence__init(array, size);
  if (!success) {
    free(array);
    return NULL;
  }
  return array;
}

void
realsense_msgs__msg__IMUInfo__Sequence__destroy(realsense_msgs__msg__IMUInfo__Sequence * array)
{
  if (array) {
    realsense_msgs__msg__IMUInfo__Sequence__fini(array);
  }
  free(array);
}
