#!/bin/bash
set -e
source /opt/ros/noetic/setup.bash
source "$HOME/catkin_ws/devel/setup.bash"


echo "Starting the complete Home Service Robot mission..."
echo "1. Display the virtual object at the pickup zone."
echo "2. Navigate to the pickup zone."
echo "3. Hide the object after successful pickup."
echo "4. Wait 5 seconds to simulate loading."
echo "5. Navigate to the drop-off zone."
echo "6. Display the object at the drop-off zone."
roslaunch pick_objects home_service.launch
