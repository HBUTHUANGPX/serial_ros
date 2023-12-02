#include "ros/ros.h"
#include "../include/serial_struct.h"
#include "../include/hardware/robot.h"
#include <iostream>
#include <thread>
int main(int argc, char **argv)
{
    ros::init(argc, argv, "my_serial_port");
    ros::NodeHandle n;
    std::vector<std::string> str;
    std::vector<std::thread> threads;
    std::vector<lively_serial *> ser;
    for (size_t i = 0; i < 4; i++)
    {
        str.push_back("/dev/ttyUSB" + std::to_string(i));
    }
    for (size_t i = 0; i < str.size(); i++)
    {
        lively_serial *s = new lively_serial(&str[i], 4000000, 1);
        ser.push_back(s);
        threads.push_back(std::thread(&lively_serial::recv, s));
    }
    ros::Rate r(100);
    robot rb(&ser);
    ROS_INFO("\033[1;32mSTART\033[0m");
    while (ros::ok())
    {
        ROS_INFO_STREAM("START");
        for (motor m : rb.Motors)
        {
            m.send(0.0, 0.0, 0.0, 0.0, 0.0);
            r.sleep();
        }
        ROS_INFO_STREAM("END"); // STEP2 -> END 1.7ms  START -> END 1.71
    }
    for (auto &thread : threads)
    {
        thread.join();
    }
    ros::spin();
    return 0;
}