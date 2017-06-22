//Not sure if this is useful
#include "ros/ros.h"
#include "tf/transform_broadcaster.h"

const float x = 0; //10cm?
const float z = 0; //20cm?
int main(int argc, char** argv){
  ros::init(argc, argv, "robot_tf_publisher");
  ros::NodeHandle n;
  ros::Rate r(100);

  tf::TransformBroadcaster broadcaster;

  while(n.ok()){
    broadcaster.sendTransform(
      tf::StampedTransform(
	tf::Transform(tf::Quaternion(0, 0, 0, 1),
	tf::Vector3(x,0,z)),
	ros::Time::now(),
	"base_link",
	"map")
      );
    r.sleep();
  }
}
