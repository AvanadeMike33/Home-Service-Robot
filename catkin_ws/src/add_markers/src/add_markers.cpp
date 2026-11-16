#include <ros/ros.h>
#include <std_msgs/Empty.h>
#include <visualization_msgs/Marker.h>

#include <string>

class MarkerManager {
 public:
  MarkerManager() : private_nh_("~") {
    private_nh_.param("pickup_x", pickup_x_, -10.0);
    private_nh_.param("pickup_y", pickup_y_, -4.0);
    private_nh_.param("dropoff_x", dropoff_x_, -5.0);
    private_nh_.param("dropoff_y", dropoff_y_, 3.0);
    private_nh_.param("marker_scale", marker_scale_, 0.40);
    private_nh_.param<std::string>("marker_frame", marker_frame_, "map");

    marker_pub_ = nh_.advertise<visualization_msgs::Marker>(
        "/visualization_marker", 1, true);
    pickup_sub_ = nh_.subscribe(
        "/pickup_reached", 1, &MarkerManager::pickupReached, this);
    dropoff_sub_ = nh_.subscribe(
        "/dropoff_reached", 1, &MarkerManager::dropoffReached, this);

    // The publisher is latched, so RViz receives the pickup marker even if it starts later.
    publishAt(pickup_x_, pickup_y_, "Oggetto al pickup");
    ROS_INFO("Marker pubblicato nella posizione di pickup.");
  }

 private:
  visualization_msgs::Marker baseMarker() const {
    visualization_msgs::Marker marker;
    marker.header.frame_id = marker_frame_;
    marker.header.stamp = ros::Time::now();
    marker.ns = "home_service_object";
    marker.id = 0;
    marker.type = visualization_msgs::Marker::CUBE;
    marker.pose.orientation.w = 1.0;
    marker.scale.x = marker_scale_;
    marker.scale.y = marker_scale_;
    marker.scale.z = marker_scale_;
    marker.color.r = 0.10f;
    marker.color.g = 0.90f;
    marker.color.b = 0.20f;
    marker.color.a = 1.0f;
    marker.lifetime = ros::Duration(0.0);
    return marker;
  }

  void publishAt(double x, double y, const std::string& text) {
    visualization_msgs::Marker marker = baseMarker();
    marker.action = visualization_msgs::Marker::ADD;
    marker.pose.position.x = x;
    marker.pose.position.y = y;
    marker.pose.position.z = marker_scale_ * 0.5;
    marker.text = text;
    marker_pub_.publish(marker);
  }

  void hideMarker() {
    visualization_msgs::Marker marker = baseMarker();
    marker.action = visualization_msgs::Marker::DELETE;
    marker_pub_.publish(marker);
  }

  void pickupReached(const std_msgs::Empty::ConstPtr&) {
    hideMarker();
    ROS_INFO("Pickup raggiunto: marker nascosto mentre il robot trasporta l'oggetto.");
  }

  void dropoffReached(const std_msgs::Empty::ConstPtr&) {
    publishAt(dropoff_x_, dropoff_y_, "Oggetto consegnato");
    ROS_INFO("Drop-off raggiunto: marker ripubblicato nella posizione finale.");
  }

  ros::NodeHandle nh_;
  ros::NodeHandle private_nh_;
  ros::Publisher marker_pub_;
  ros::Subscriber pickup_sub_;
  ros::Subscriber dropoff_sub_;
  double pickup_x_;
  double pickup_y_;
  double dropoff_x_;
  double dropoff_y_;
  double marker_scale_;
  std::string marker_frame_;
};

int main(int argc, char** argv) {
  ros::init(argc, argv, "add_markers");
  MarkerManager manager;
  ros::spin();
  return 0;
}
