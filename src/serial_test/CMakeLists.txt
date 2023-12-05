cmake_minimum_required(VERSION 3.0.2)
project(serial_test)

# add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  serial
  pai_msgs
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES serial_test
 CATKIN_DEPENDS roscpp rospy std_msgs pai_msgs
#  DEPENDS system_lib
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)
file(GLOB_RECURSE SRC_LIST
"src/*.cc"
)
add_executable(serial_t src/serial_t.cpp ${SRC_LIST} )
target_link_libraries(serial_t ${catkin_LIBRARIES})