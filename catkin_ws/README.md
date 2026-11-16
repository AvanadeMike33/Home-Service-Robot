# Home Service Robot

## Project Overview

This project implements a simulated Home Service Robot using ROS Noetic and Gazebo in the custom `AvanadeOffice` environment.

The system supports:
- Gazebo simulation of the robot and environment
- SLAM and occupancy-grid map generation
- AMCL localization
- autonomous navigation with `move_base`
- automatic pickup and drop-off goals
- virtual-object visualization in RViz
- a complete home-service mission

## ROS Packages

### `my_robot`
Contains the robot model, Gazebo world, sensors, saved map, SLAM/localization launch files, and navigation-stack configuration. The robot uses differential drive, wheel odometry and a Hokuyo laser scanner.

### `pick_objects`
Sends pickup and drop-off goals to the `move_base` action server. The required mission behavior is: travel to the pickup zone, report arrival, wait 5 seconds to simulate loading, then travel to the drop-off zone and report arrival.

### `add_markers`
Publishes the virtual object on `/visualization_marker`. The object is shown at the pickup zone, hidden after pickup, and shown again at the drop-off zone after delivery.

## Mapping

Run:

```bash
./scripts/test_slam.sh
```

The script launches the robot and SLAM configuration, starts RViz, and enables keyboard teleoperation. Drive slowly through the environment and verify that `/map` is generated and updated.

## Localization and Navigation

The saved occupancy map is loaded by `map_server`. AMCL uses the saved map, `/scan` and `/odom` to estimate the robot pose in the `map` frame. `move_base` performs global and local path planning.

Run:

```bash
./scripts/test_navigation.sh
```

In RViz, use **2D Pose Estimate** to initialize localization if required, then use **2D Nav Goal** to verify autonomous navigation.

The expected TF chain is:

```text
map -> odom -> robot_footprint -> chassis -> hokuyo
```

## Pick Objects Test

Run:

```bash
./scripts/pick_objects.sh
```

Expected sequence:
1. Navigate to the pickup zone.
2. Print a pickup-arrival message.
3. Wait 5 seconds to simulate loading.
4. Navigate to the drop-off zone.
5. Print a drop-off-arrival message.

## Marker Test

Run:

```bash
./scripts/add_marker.sh
```

In RViz, add a `Marker` display using `/visualization_marker`. The marker should start at the pickup zone, disappear after pickup, and reappear at the drop-off zone.

## Complete Home Service Mission

Run:

```bash
./scripts/home_service.sh
```

Expected behavior:
1. The virtual object is visible at the pickup zone.
2. The robot navigates to the pickup zone.
3. The object disappears after successful pickup.
4. The robot waits 5 seconds to simulate loading.
5. The robot navigates to the drop-off zone.
6. The object reappears at the drop-off zone.

## Build

```bash
cd ~/catkin_ws
source /opt/ros/noetic/setup.bash
catkin_make
source devel/setup.bash
chmod +x ~/catkin_ws/src/scripts/*.sh
```

## Required Scripts

```text
scripts/
├── test_slam.sh
├── test_navigation.sh
├── pick_objects.sh
├── add_marker.sh
└── home_service.sh
```

## Project Structure

```text
src/
├── my_robot/
│   ├── launch/
│   ├── urdf/
│   ├── worlds/
│   ├── maps/
│   ├── config/
│   ├── meshes/
│   ├── CMakeLists.txt
│   └── package.xml
├── pick_objects/
│   ├── launch/
│   ├── src/
│   ├── CMakeLists.txt
│   └── package.xml
├── add_markers/
│   ├── launch/
│   ├── src/
│   ├── CMakeLists.txt
│   └── package.xml
├── scripts/
│   ├── test_slam.sh
│   ├── test_navigation.sh
│   ├── pick_objects.sh
│   ├── add_marker.sh
│   └── home_service.sh
└── README.md
```

## Software
- Ubuntu 20.04
- ROS Noetic
- Gazebo 11
- RViz
- ROS Navigation Stack
