#include"Ptzclass.h"
#include<ros/ros.h>
#include<sensor_msgs/Joy.h>
#include<std_msgs/Int32MultiArray.h>
using namespace std;
//0位控制俯仰，1位控制旋转
void control_callBack(const sensor_msgs::Joy::ConstPtr& msg){
    PTZ ptz;
    ptz.Ptz_relativeControl(msg->axes[0]*5,msg->axes[1]*5,0);
    // if(1==msg->buttons[0]){
    //     ptz.Ptz_absoluteControl(0,0,0);
    // }  
}

void absoluteControl_callBack(const std_msgs::Int32MultiArray msg){
    PTZ ptz;
    ptz.Ptz_absoluteControl(msg.data[0],msg.data[1],0);  
}

int main(int argc ,char **argv){
    ros::init(argc,argv,"ptz_control_node");
    ros::NodeHandle nh;
    ros::Subscriber sub=nh.subscribe<sensor_msgs::Joy>("/ptz_control",20,control_callBack);
    ros::Subscriber sub2=nh.subscribe<std_msgs::Int32MultiArray>("/ptz_absoluteControl",20,absoluteControl_callBack);
    ros::spin();
    return 0;
}