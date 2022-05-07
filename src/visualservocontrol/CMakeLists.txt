cmake_minimum_required(VERSION 3.5)
project(visualservocontrol)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(FastRTPS_INCLUDE_DIR /opt/ros/foxy/include)
set(FastRTPS_LIBRARY_RELEASE /opt/ros/foxy/lib/libfastrtps.so)

# find dependencies

# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

#添加执行文件
aux_source_directory(./src SRC_LIST)

set(SRC_LIST_WITHOUTE)
#file(READ "src/*.cpp" SRC_LIST)
#去掉扩展名
foreach(SRC_FILE ${SRC_LIST})
  get_filename_component(SRC_FILE_WITHOUTE ${SRC_FILE} NAME_WLE)
  set(SRC_LIST_WITHOUTE ${SRC_LIST_WITHOUTE} ${SRC_FILE_WITHOUTE} )
endforeach()
message(STATUS "${SRC_LIST_WITHOUTE}")
foreach(SRC_FILE ${SRC_LIST_WITHOUTE})
  add_executable(${SRC_FILE} src/${SRC_FILE}.cpp)
endforeach()
#add_executable(ObjectRecognizer src/ObjectRecognizer.cpp)
#add_executable(PhotoCapture src/PhotoCapture.cpp)
#基础库
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(image_transport REQUIRED)

foreach(TARGET_NAME ${SRC_LIST_WITHOUTE})
  target_include_directories(${TARGET_NAME} PUBLIC
          $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
          $<INSTALL_INTERFACE:include>)
endforeach()
set(AMENT_DEPENDENCIES ${AMENT_DEPENDENCIES} rclcpp std_msgs sensor_msgs cv_bridge image_transport)

#寻找opencv库
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
set(MYLIBRARIES ${MYLIBRARIES} ${OpenCV_LIBS})
#寻找eigen库
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIRS})
#message(STATUS "${EIGEN3_INCLUDE_DIRS}")
#寻找VISP库
#find_package(VISP REQUIRED visp_core visp_robot visp_vs visp_blob visp_vision visp_io visp_gui visp_ar)
find_package(VISP REQUIRED)
find_package(Boost COMPONENTS thread date_time REQUIRED)
set(MYLIBRARIES ${MYLIBRARIES} ${VISP_LIBRARIES} Boost::date_time Boost::thread)
#foreach (_variableName ${VISP_LIBRARIES})
#  message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()
#set(MYLIBRARIES ${MYLIBRARIES} "/home/wzf/Documents/visp-ws/visp-build/lib/libvisp_vs.so.3.3" Boost::date_time Boost::thread)
include_directories(${VISP_INCLUDE_DIRS})
#PCL 库
find_package( PCL 1.11.1 REQUIRED )
include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})
set(MYLIBRARIES ${MYLIBRARIES} ${PCL_LIBRARIES})
#寻找OPENGL库
find_package(OpenGL)
set(MYLIBRARIES ${MYLIBRARIES} ${OPENGL_LIBRARIES})
#寻找glfw3库
find_package(glfw3 REQUIRED)
set(MYLIBRARIES ${MYLIBRARIES} glfw)
#realsense 库
find_package(realsense_ros PATHS "/home/wzf/main_folder/MyProject/Graduation_project/install")
find_package(realsense_msgs PATHS "/home/wzf/main_folder/MyProject/Graduation_project/install")
find_package(nav_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(pcl_conversions REQUIRED)
find_library(realsense2_LIBRARIES realsense2)
find_path(realsense2_INCLUDE_DIRS librealsense2/rs.hpp)
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${realsense2_INCLUDE_DIRS}
)
set(AMENT_DEPENDENCIES ${AMENT_DEPENDENCIES}
        pcl_conversions
        nav_msgs
        tf2
        tf2_ros
        realsense_ros
        realsense_msgs)

include_directories("/home/wzf/main_folder/librealsense/examples")
set(MYLIBRARIES ${MYLIBRARIES} realsense2)
#寻找HDF5 FLANN库
find_package(HDF5 REQUIRED)
find_package(FLANN REQUIRED)

include_directories(SYSTEM
        ${HDF5_INCLUDE_DIR}
        )
set(MYLIBRARIES ${MYLIBRARIES} FLANN::FLANN ${HDF5_LIBRARIES})
# 依赖库链接安装等
install(
        DIRECTORY include/
        DESTINATION include
)
install(
        DIRECTORY launch/
        DESTINATION share/${PROJECT_NAME}/launch
)
foreach(TARGET_NAME ${SRC_LIST_WITHOUTE})
  ament_target_dependencies(${TARGET_NAME} ${AMENT_DEPENDENCIES})
  target_link_libraries(${TARGET_NAME} ${MYLIBRARIES})
  install(TARGETS ${TARGET_NAME}
          DESTINATION lib/${PROJECT_NAME})
endforeach()



if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # uncomment the line when a copyright and license is not present in all source files
  #set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # uncomment the line when this package is not in a git repo
  #set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
