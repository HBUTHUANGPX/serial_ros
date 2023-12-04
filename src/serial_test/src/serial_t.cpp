#include "ros/ros.h"
#include "../include/serial_struct.h"
#include "../include/hardware/robot.h"
#include <iostream>
#include <thread>
#include <condition_variable>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "my_serial_port");
    ros::NodeHandle n;
    ros::Rate r(500);

    std::mutex mtx;
    std::condition_variable cv;
    robot rb(&cv, &mtx);
    ROS_INFO("\033[1;32mSTART\033[0m");
    // ========================== singlethread send =====================
    while (ros::ok()) // 此用法为逐个电机发送控制指令
    {
        // ROS_INFO_STREAM("START");
        for (motor &m : rb.Motors)
        {
            m.send(0.0, 0.0, 0.0, 0.0, 0.0);
        }
        // ROS_INFO_STREAM("END"); // STEP2 -> END 1.7ms  START -> END 1.71
        r.sleep();
    }
    // ============================ multithread send =====================
    // rb.init_mutithread_send();
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

    for (auto &thread : rb.ser_recv_threads)
    {
        thread.join();
    }
    ros::spin();
    return 0;
}
