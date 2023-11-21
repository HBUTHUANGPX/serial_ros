#include "ros/ros.h"
#include "../include/serial_struct.h"


int main(int argc, char **argv)
{
    ros::init(argc, argv, "my_serial_port_imu");
    ros::NodeHandle n;
    std::string str = "/dev/ttyACM0";
    lively_serial a(&str,921600,1);
    a.send(0x01,0,0,0,0,1);
}