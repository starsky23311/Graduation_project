cmake_minimum_required(VERSION 3.5)
project(qt_plot)

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


set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
aux_source_directory(./src SRC_LIST)


message(STATUS "${SRC_LIST}")

# 寻找ros库
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(image_transport REQUIRED)
# 寻找Qt库
find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Gui)
find_package(Qt5OpenGL)
find_package(Qt5Xml)
find_package(Qt5PrintSupport REQUIRED)
include_directories(${Qt5Core_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS} ${Qt5OpenGL_INCLUDE_DIRS} ${Qt5Xml_INCLUDE_DIRS})
#寻找opencv库
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
#qt5_wrap_ui(ui_FILES mainwindow.ui)
#qt5_add_resources(qrc_FILES drawable.qrc)
#add_executable(${PROJECT_NAME} ${SRC_LIST} ${ui_FILES} ${qrc_FILES})


# 可执行文件
#qt5_wrap_cpp(SRC_LIST ./src)
#add_library(mylibrary ${PROJECT_SOURCE_DIR}/include/qt_plot/mainwindow.h)
add_executable(myplot ${SRC_LIST}
        ${PROJECT_SOURCE_DIR}/include/qt_plot/qcustomplot.h
        ${PROJECT_SOURCE_DIR}/include/qt_plot/mainwindow.h
        ${PROJECT_SOURCE_DIR}/src/mainwindow.ui)
ament_target_dependencies(myplot rclcpp std_msgs sensor_msgs cv_bridge image_transport)


#包含路径
target_include_directories(myplot PUBLIC ${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/qt_plot)
#链接库
target_include_directories(myplot PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)
qt5_use_modules(myplot Core Widgets OpenGL Xml Gui PrintSupport) # Qt
target_link_libraries(myplot ${OpenCV_LIBS})
#安装库
install(
        DIRECTORY include/
        DESTINATION include
)
install(TARGETS myplot
  DESTINATION lib/${PROJECT_NAME})

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
