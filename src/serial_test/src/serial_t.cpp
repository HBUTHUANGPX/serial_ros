#include "ros/ros.h"
#include "../include/serial_struct.h"
#include "../include/hardware/robot.h"
#include <iostream>
#include <thread>
#include <condition_variable>
std::mutex mtx;
std::condition_variable cv;
bool sendEnabled = true;
void fresh()
{
    ROS_INFO("===1===");
}
int main(int argc, char **argv)
{
    ros::init(argc, argv, "my_serial_port");
    ros::NodeHandle n;
    std::vector<std::string> str;
    std::vector<std::thread> ser_recv_threads, send_threads;
    std::vector<lively_serial *> ser;
    for (size_t i = 0; i < 4; i++)
    {
        str.push_back("/dev/ttyUSB" + std::to_string(i));
    }
    for (size_t i = 0; i < str.size(); i++)
    {
        lively_serial *s = new lively_serial(&str[i], 4000000, 1);
        ser.push_back(s);
        ser_recv_threads.push_back(std::thread(&lively_serial::recv, s));
    }

    ros::Rate r(1000);
    robot rb(&ser, &cv,&mtx);
    // for (canport &cp : rb.CANPorts)
    // {
    //     ROS_INFO("%d %d",cp.get_canboard_id(),cp.get_canport_id());
    //     send_threads.push_back(std::thread(&canport::send_d,&cp));
    //     r.sleep();
    // }
    ROS_INFO("\033[1;32mSTART\033[0m");

    // while (ros::ok())// 此用法为逐个电机多个串口并行发送控制指令
    // {
    //     // ROS_INFO_STREAM("START");
    //     ROS_INFO("===0===");
    //     for (motor &m : rb.Motors)
    //     {
    //         m.fresh_cmd(0.0, 0.0, 0.0, 0.0, 0.0);
    //     }
    //     rb.enable_send();
    //     // rb.CANPorts[0].enable_send();
    //     cv.notify_all(); // 通知子线程可以发送数据了
    //     ROS_INFO_STREAM("note"); // STEP2 -> END 1.7ms  START -> END 1.71
    //     r.sleep();
    // }
    while (ros::ok()) // 此用法为逐个电机发送控制指令
    {
        // ROS_INFO_STREAM("START");
        for (motor &m : rb.Motors)
        {
            m.send(0.0, 0.0, 1.0, 0.0, 0.0);
        }
        // ROS_INFO_STREAM("END"); // STEP2 -> END 1.7ms  START -> END 1.71
        r.sleep();
    }
    for (auto &thread : ser_recv_threads)
    {
        thread.join();
    }
    ros::spin();
    return 0;
}
