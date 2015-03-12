#include "ros/ros.h"
#include "std_msgs/Empty.h"
#include "geometry_msgs/Twist.h"
#include "ardrone_autonomy/Navdata.h"
#include <sstream>
#include <math.h>

ardrone_autonomy::Navdata msg_in;

void read_navdata(const ardrone_autonomy::Navdata::ConstPtr& msg)
{ 
  msg_in.altd = msg->altd;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "talker");
  ros::NodeHandle n;
  
  std_msgs::Empty my_msg;  
  geometry_msgs::Twist m;
  ros::Publisher takeoff = n.advertise<std_msgs::Empty>("/ardrone/takeoff", 10,true);
  takeoff.publish(my_msg);
  
  ros::Publisher twist;
  twist = n.advertise<geometry_msgs::Twist>("/quad/cmd_vel", 1);

  ros::Subscriber read = n.subscribe("/ardrone/navdata", 1000, read_navdata);  
	
  int c;

      for(c=0;c<10;c++)
      c++;

       ros::Rate loop_rate(10);

       m.linear.z = 0.1;
       m.linear.y = 0;
       m.angular.x = 0;
       m.angular.y = 0;
       m.angular.z = 0;
       m.linear.x = 0;  
       
       float z1;
        
       float z = 1000.0; 
       z1 = msg_in.altd;
       float error,pre_error,integral,derivative;
       integral =0;
       
       float kp,ki,kd;
       kp = 0.25;
       ki = 0.001;
       kd = 0.0; 
 
       while (ros::ok())
       {    
       
       error = (z - z1)/70;
       integral = integral + error;
       derivative = error-pre_error;
       m.linear.z = kp*error + ki*integral + kd*derivative;
       ROS_INFO("%d", msg_in.altd);
       twist.publish(m);
       pre_error = error;
       z1 = msg_in.altd;
       ros::spinOnce(); 
       loop_rate.sleep();
     
     }
  
  
    return 0;
  }
