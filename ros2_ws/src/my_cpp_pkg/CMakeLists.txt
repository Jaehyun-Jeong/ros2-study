cmake_minimum_required(VERSION 3.8)
project(my_cpp_pkg)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(example_interfaces REQUIRED)
find_package(my_robot_interfaces REQUIRED)

add_executable(hw_status_publisher src/hardware_status_publisher.cpp)
ament_target_dependencies(hw_status_publisher rclcpp my_robot_interfaces)

install(TARGETS
  hw_status_publisher
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
