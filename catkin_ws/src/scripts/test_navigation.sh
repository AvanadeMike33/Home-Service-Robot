#!/bin/bash
set -e
source /opt/ros/noetic/setup.bash
source "$HOME/catkin_ws/devel/setup.bash"


echo "Starting simulation, localization and navigation..."
roslaunch my_robot test_navigation.launch
