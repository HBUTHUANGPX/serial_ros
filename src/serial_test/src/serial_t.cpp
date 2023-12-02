#include "ros/ros.h"
#include "../include/serial_struct.h"
#include "../include/hardware/robot.h"
#include <iostream>
#include <thread>

void read_params()
{
    ros::NodeHandle n;
    std::string robot_name;
    // if (n.getParam("robot/robot_name",robot_name)){ROS_INFO("Got params: %s",robot_name.c_str());}else{ROS_ERROR("Faile to get params");}
    // int CANboard_num;
    // if (n.getParam("robot/CANboard",CANboard_num)){ROS_INFO("Got params: %d",CANboard_num);}else{ROS_ERROR("Faile to get params");}
    // for (size_t i = 0; i < CANboard_num; i++)
    // {
    //     /* code */
    // }
}
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
            m.send(0, 0, 0, 30000, 0);
            r.sleep();
        }
        ROS_INFO_STREAM("END"); // STEP2 -> END 1.7ms  START -> END 1.71
    }
    //////
    for (auto &thread : threads)
    {
        thread.join();
    }

    ros::spin();
    // a.send(0x04,0,0,0,30000,0);
    // a.send(0x03,0,0,0,30000,0);
    // a.send(0x02,0,0,0,30000,0);
    // a.send(0x01,0,0,0,30000,0);
    return 0;
}