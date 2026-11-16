cmake_minimum_required(VERSION 3.0.2)
project(pick_objects)

add_compile_options(-std=c++14 -Wall -Wextra)

find_package(catkin REQUIRED COMPONENTS
  actionlib
  move_base_msgs
  roscpp
  std_msgs
)

catkin_package(
  CATKIN_DEPENDS actionlib move_base_msgs roscpp std_msgs
)

include_directories(${catkin_INCLUDE_DIRS})

add_executable(pick_objects_node src/pick_objects.cpp)
target_link_libraries(pick_objects_node ${catkin_LIBRARIES})
add_dependencies(pick_objects_node ${catkin_EXPORTED_TARGETS})

install(TARGETS pick_objects_node
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
install(DIRECTORY launch
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
