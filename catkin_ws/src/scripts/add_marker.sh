#!/bin/bash
set -e
source /opt/ros/noetic/setup.bash
source "$HOME/catkin_ws/devel/setup.bash"


echo "Starting the virtual object marker test..."
roslaunch add_markers add_marker.launch
