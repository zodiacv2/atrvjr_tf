#include "ros/ros.h"
#include "tf/transform_broadcaster.h"
#include "nav_msgs/Odometry.h"

int main(int argc, char** argv){
  ros::init(argc, argv, "odometry_publisher");

  ros::NodeHandle n;
  ros::Publisher odomPub = n.advertise<nav_msgs::Odometry>("odom", 50);
  tf::TransformBroadcaster odomBroadcaster;

  double x = 0.0;
  double y = 0.0;
  double th = 0.0;

  double vx = 0.1;
  double vy = -0.1;
  double vth = 0.1;

  ros::Time current_time, last_time;
  current_time = ros::Time::now();
  last_time = ros::Time::now();

  ros::Rate r(1.0);
  while(n.ok()){
    ros::spinOnce();
    current_time = ros::Time::now();

    double dt = (current_time - last_time).toSec();
    double delta_x = (vx * cos(th) - vy * sin(th)) * dt;
    double delta_y = (vx * sin(th) + vy * cos(th)) * dt;
    double delta_th = vth * dt;
						  
    x += delta_x;
    y += delta_y;
    th += delta_th;
						  
    geometry_msgs::Quaternion odomQuaternion = tf::createQuaternionMsgFromYaw(th);
						
    geometry_msgs::TransformStamped odomTransform;
    odomTransform.header.stamp = current_time;
    odomTransform.header.frame_id = "odom";
    odomTransform.child_frame_id = "base_link";
    odomTransform.transform.translation.x = x;
    odomTransform.transform.translation.y = y;
    odomTransform.transform.translation.z = 0.0;
    odomTransform.transform.rotation = odomQuaternion;
			 
    odomBroadcaster.sendTransform(odomTransform);
						
    nav_msgs::Odometry odom;
    odom.header.stamp = current_time;
    odom.header.frame_id = "odom";
						  
    odom.pose.pose.position.x = x;
    odom.pose.pose.position.y = y;
    odom.pose.pose.position.z = 0.0;
    odom.pose.pose.orientation = odomQuaternion;
    odom.child_frame_id = "base_link";
    odom.twist.twist.linear.x = vx;
    odom.twist.twist.linear.y = vy;
    odom.twist.twist.angular.z = vth;

    odomPub.publish(odom);
							 
    last_time = current_time;
    r.sleep();
  }
}
