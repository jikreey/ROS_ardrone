cmake_minimum_required(VERSION 2.8.3)
project(beginner_tutorials)

find_package(catkin REQUIRED COMPONENTS
  cv_bridge
  image_transport
  roscpp
  sensor_msgs
)

find_package( OpenCV REQUIRED )

catkin_package(
CATKIN_DEPENDS cv_bridge image_transport roscpp sensor_msgs
)

include_directories(include)
include_directories(
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)

add_executable(exe src/main.cpp)
add_dependencies(exe beginner_tutorials_generate_messages_cpp)
target_link_libraries(exe 
  ${catkin_LIBRARIES}
  ${OpenCV_LIBS}
)
