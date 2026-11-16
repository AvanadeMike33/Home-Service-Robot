#!/bin/bash
set -e
source /opt/ros/noetic/setup.bash
source "$HOME/catkin_ws/devel/setup.bash"


echo "Starting automated pickup and drop-off navigation..."
roslaunch pick_objects pick_objects.launch
