cmake_minimum_required(VERSION 3.0.2)
project(add_markers)

add_compile_options(-std=c++14 -Wall -Wextra)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  visualization_msgs
)

catkin_package(
  CATKIN_DEPENDS roscpp std_msgs visualization_msgs
)

include_directories(${catkin_INCLUDE_DIRS})

add_executable(add_markers_node src/add_markers.cpp)
target_link_libraries(add_markers_node ${catkin_LIBRARIES})
add_dependencies(add_markers_node ${catkin_EXPORTED_TARGETS})

install(TARGETS add_markers_node
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
install(DIRECTORY launch
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
