#include "ros/ros.h"
#include "../include/serial_struct.h"
#include <iostream>
#include <thread>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "my_serial_port");
    ros::NodeHandle n;
    // std::string str = "/dev/ttyACM0";
    std::string str0 = "/dev/ttyUSB0";
    lively_serial a(&str0, 4000000, 1);
    std::string str1 = "/dev/ttyUSB1";
    lively_serial b(&str1, 4000000, 1);
    std::thread newthread_a(&lively_serial::recv, &a);
    std::thread newthread_b(&lively_serial::recv, &a);

    // a.send(0x04, 0, 0, 0, 30000, 0);
    // a.send(0x04, 0, 0, 0, 30000, 0);
    ros::Rate r(100);
    while (ros::ok())
    {
        ROS_INFO_STREAM("START");
        a.send(0x04, 0, 0, 0, 30000, 0);
        ROS_INFO_STREAM("mid");
        b.send(0x04, 0, 0, 0, 30000, 0);
        ROS_INFO_STREAM("END"); // STEP2 -> END 1.7ms  START -> END 1.71
        r.sleep();
    }
    //////
    newthread_a.join();
    newthread_b.join();

    ros::spin();
    // a.send(0x04,0,0,0,30000,0);
    // a.send(0x03,0,0,0,30000,0);
    // a.send(0x02,0,0,0,30000,0);
    a.send(0x01,0,0,0,30000,0);
    return 0;
}