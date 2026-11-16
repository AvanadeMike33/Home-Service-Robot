#!/bin/bash
set -e
source /opt/ros/noetic/setup.bash
source "$HOME/catkin_ws/devel/setup.bash"


echo "Starting Gazebo and SLAM..."
roslaunch my_robot slam.launch &
SLAM_PID=$!
cleanup(){ echo; echo "Stopping SLAM test..."; kill "$SLAM_PID" 2>/dev/null || true; }
trap cleanup EXIT INT TERM

echo "Waiting for the laser scanner..."
until rostopic list 2>/dev/null | grep -q '^/scan$'; do sleep 1; done
echo "Laser scanner detected."
echo "Waiting for the map..."
until rostopic list 2>/dev/null | grep -q '^/map$'; do sleep 1; done
echo "Map topic detected."
echo "Starting RViz..."
rviz &
sleep 3

echo "Starting keyboard teleoperation."
echo "Controls: i forward, , backward, j left, l right, k stop"
rosrun teleop_twist_keyboard teleop_twist_keyboard.py
wait "$SLAM_PID"
