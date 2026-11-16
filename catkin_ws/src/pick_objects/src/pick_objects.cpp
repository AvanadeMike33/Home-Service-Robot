#include <actionlib/client/simple_action_client.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <ros/ros.h>
#include <std_msgs/Empty.h>

#include <cmath>
#include <string>

using MoveBaseClient = actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction>;

move_base_msgs::MoveBaseGoal makeGoal(double x, double y, double yaw) {
  move_base_msgs::MoveBaseGoal goal;
  goal.target_pose.header.frame_id = "map";
  goal.target_pose.header.stamp = ros::Time::now();
  goal.target_pose.pose.position.x = x;
  goal.target_pose.pose.position.y = y;
  goal.target_pose.pose.position.z = 0.0;
  goal.target_pose.pose.orientation.x = 0.0;
  goal.target_pose.pose.orientation.y = 0.0;
  goal.target_pose.pose.orientation.z = std::sin(yaw * 0.5);
  goal.target_pose.pose.orientation.w = std::cos(yaw * 0.5);
  return goal;
}

bool reachGoal(MoveBaseClient& client,
               const move_base_msgs::MoveBaseGoal& goal,
               const std::string& label,
               int attempts,
               double timeout_seconds,
               double retry_pause_seconds) {
  for (int attempt = 1; attempt <= attempts && ros::ok(); ++attempt) {
    move_base_msgs::MoveBaseGoal stamped_goal = goal;
    stamped_goal.target_pose.header.stamp = ros::Time::now();

    ROS_INFO_STREAM("Invio goal " << label << " (tentativo " << attempt
                    << "/" << attempts << ")");
    client.sendGoal(stamped_goal);

    const bool finished = client.waitForResult(ros::Duration(timeout_seconds));
    if (finished && client.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
      ROS_INFO_STREAM("Goal " << label << " raggiunto.");
      return true;
    }

    if (!finished) {
      ROS_WARN_STREAM("Timeout durante il goal " << label << ".");
    } else {
      ROS_WARN_STREAM("Goal " << label << " terminato con stato: "
                      << client.getState().toString());
    }
    client.cancelGoal();

    if (attempt < attempts) {
      ROS_INFO_STREAM("Nuovo tentativo tra " << retry_pause_seconds << " secondi.");
      ros::Duration(retry_pause_seconds).sleep();
    }
  }
  return false;
}

int main(int argc, char** argv) {
  ros::init(argc, argv, "pick_objects");
  ros::NodeHandle nh;
  ros::NodeHandle private_nh("~");

  double pickup_x = -10.0;
  double pickup_y = -4.0;
  double pickup_yaw = 3.141592653589793;
  double dropoff_x = -5.0;
  double dropoff_y = 3.0;
  double dropoff_yaw = 1.5707963267948966;
  double pickup_wait_seconds = 5.0;
  double goal_timeout_seconds = 180.0;
  double retry_pause_seconds = 2.0;
  int goal_attempts = 3;

  private_nh.param("pickup_x", pickup_x, pickup_x);
  private_nh.param("pickup_y", pickup_y, pickup_y);
  private_nh.param("pickup_yaw", pickup_yaw, pickup_yaw);
  private_nh.param("dropoff_x", dropoff_x, dropoff_x);
  private_nh.param("dropoff_y", dropoff_y, dropoff_y);
  private_nh.param("dropoff_yaw", dropoff_yaw, dropoff_yaw);
  private_nh.param("pickup_wait_seconds", pickup_wait_seconds, pickup_wait_seconds);
  private_nh.param("goal_timeout_seconds", goal_timeout_seconds, goal_timeout_seconds);
  private_nh.param("retry_pause_seconds", retry_pause_seconds, retry_pause_seconds);
  private_nh.param("goal_attempts", goal_attempts, goal_attempts);

  ros::Publisher pickup_reached_pub =
      nh.advertise<std_msgs::Empty>("/pickup_reached", 1, true);
  ros::Publisher dropoff_reached_pub =
      nh.advertise<std_msgs::Empty>("/dropoff_reached", 1, true);

  MoveBaseClient move_base_client("move_base", true);
  ROS_INFO("Attendo il server action move_base...");
  while (ros::ok() && !move_base_client.waitForServer(ros::Duration(5.0))) {
    ROS_WARN("move_base non e' ancora disponibile; continuo ad attendere.");
  }
  if (!ros::ok()) {
    return 1;
  }

  // Give event subscribers a short wall-clock interval to connect.
  ros::WallDuration(1.0).sleep();

  const move_base_msgs::MoveBaseGoal pickup_goal =
      makeGoal(pickup_x, pickup_y, pickup_yaw);
  if (!reachGoal(move_base_client, pickup_goal, "PICKUP", goal_attempts,
                 goal_timeout_seconds, retry_pause_seconds)) {
    ROS_ERROR("Impossibile raggiungere il pickup dopo tutti i tentativi.");
    return 2;
  }

  std_msgs::Empty event;
  pickup_reached_pub.publish(event);
  ROS_INFO("Pickup raggiunto: oggetto raccolto. Attesa di 5 secondi.");
  ros::Duration(pickup_wait_seconds).sleep();

  const move_base_msgs::MoveBaseGoal dropoff_goal =
      makeGoal(dropoff_x, dropoff_y, dropoff_yaw);
  if (!reachGoal(move_base_client, dropoff_goal, "DROP-OFF", goal_attempts,
                 goal_timeout_seconds, retry_pause_seconds)) {
    ROS_ERROR("Impossibile raggiungere il drop-off dopo tutti i tentativi.");
    return 3;
  }

  dropoff_reached_pub.publish(event);
  ROS_INFO("Drop-off raggiunto: oggetto consegnato.");
  ROS_INFO("Sequenza completata; il nodo resta attivo per mantenere gli eventi latched.");
  ros::spin();
  return 0;
}
