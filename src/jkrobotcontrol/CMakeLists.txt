cmake_minimum_required(VERSION 3.5)
project(jkrobotcontrol)

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
# 将./src/ 下的文件列成表
#aux_source_directory(./src SRC_LIST)
#set(SRC_LIST_WITHOUTE)
set(SRC_LIST src/keyboard.cpp src/robotState.cpp src/TCPClient.cpp src/TCPServer.cpp)
#file(READ "src/*.cpp" SRC_LIST)
#去掉扩展名
#foreach(SRC_FILE ${SRC_LIST})
#  get_filename_component(SRC_FILE_WITHOUTE ${SRC_FILE} NAME_WLE)
#  set(SRC_LIST_WITHOUTE ${SRC_LIST_WITHOUTE} ${SRC_FILE_WITHOUTE} )
#endforeach()

#foreach(SRC_FILE ${SRC_LIST_WITHOUTE})
 # add_executable(${SRC_FILE} src/${SRC_FILE}.cpp)
#endforeach()

#基础库
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
set(AMENT_DEPENDENCIES rclcpp std_msgs sensor_msgs nav_msgs tf2 tf2_ros)
#寻找eigen库
find_package(Eigen3 REQUIRED)


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
#包含头文件路径
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${EIGEN3_INCLUDE_DIRS}
)

# 依赖库链接安装等
add_executable(main src/main.cpp)
add_library (MathFunctions SHARED ${SRC_LIST})

target_include_directories(main PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)


target_include_directories(MathFunctions PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)
#link_directories(
#        ${COLCON_LIB_DIRS}  lib)
#ament_export_libraries(MathFunctions)
#find_package(MathFunctions REQUIRED)


#ament_target_dependencies(MathFunctions PUBLIC ${AMENT_DEPENDENCIES})
#ament_export_targets(MathFunctions HAS_LIBRARY_TARGET)
#ament_export_dependencies(rclcpp)
install(
        DIRECTORY include/
        DESTINATION include
)
install(TARGETS MathFunctions
        EXPORT  MathFunctions
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
        )


#link_directories(/home/wzf/main_folder/CLionProjects/JK_project/install/jkrobotcontrol/lib/jkrobotcontrol)
ament_target_dependencies(main ${AMENT_DEPENDENCIES})
target_link_libraries(main MathFunctions)
install(TARGETS main
  DESTINATION lib/${PROJECT_NAME})






ament_package()
