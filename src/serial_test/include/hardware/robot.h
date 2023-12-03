#ifndef _ROBOT_H_
#define _ROBOT_H_
#include <iostream>
#include "canboard.h"
#include "ros/ros.h"
#include <thread>
class robot
{
private:
    std::string robot_name, Serial_Type, CAN_type, CANboard_type, Serial_allocate;
    int arm_dof, leg_dof, CANboard_num;
    ros::NodeHandle n;
    std::vector<canboard> CANboards;

public:
    std::vector<motor> Motors;
    // std::vector<std::shared_ptr<canport>> CANPorts;
    std::vector<canport> CANPorts;

    robot(std::vector<lively_serial *> *ser,std::condition_variable *_cv,std::mutex *_mtx)
    {
        if (n.getParam("robot/robot_name", robot_name))
        {
            // ROS_INFO("Got params robot_name: %s",robot_name.c_str());
        }
        else
        {
            ROS_ERROR("Faile to get params robot_name");
        }
        if (n.getParam("robot/CANboard_num", CANboard_num))
        {
            // ROS_INFO("Got params CANboard_num: %d",CANboard_num);
        }
        else
        {
            ROS_ERROR("Faile to get params CANboard_num");
        }
        if (n.getParam("robot/CANboard_type", CANboard_type))
        {
            // ROS_INFO("Got params CANboard_type: %s",CANboard_type.c_str());
        }
        else
        {
            ROS_ERROR("Faile to get params CANboard_type");
        }
        if (n.getParam("robot/CANboard_type", CANboard_type))
        {
            // ROS_INFO("Got params CANboard_type: %s",CANboard_type.c_str());
        }
        else
        {
            ROS_ERROR("Faile to get params CANboard_type");
        }
        if (n.getParam("robot/Serial_Type", Serial_Type))
        {
            // ROS_INFO("Got params Serial_Type: %s",Serial_Type.c_str());
        }
        else
        {
            ROS_ERROR("Faile to get params Serial_Type");
        }
        if (n.getParam("robot/Serial_allocate", Serial_allocate))
        {
            // ROS_INFO("Got params Serial_Type: %s",Serial_Type.c_str());
        }
        else
        {
            ROS_ERROR("Faile to get params Serial_allocate");
        }
        if (Serial_allocate == "1for2")
        {
            for (size_t i = 1; i <= CANboard_num; i++) // 一个CANboard使用两个串口
            {
                CANboards.push_back(canboard(i, ser,_cv,_mtx));
            }
        }

        ROS_INFO("\033[1;32mThe robot name is %s\033[0m", robot_name.c_str());
        ROS_INFO("\033[1;32mThe robot has %d CANboards\033[0m", CANboard_num);
        ROS_INFO("\033[1;32mThe CANboard type is %s\033[0m", CANboard_type.c_str());
        ROS_INFO("\033[1;32mThe Serial type is %s\033[0m", Serial_Type.c_str());
        ROS_INFO("\033[1;32mThe Serial allocate type is %s\033[0m", Serial_allocate.c_str());

        for (canboard &cb : CANboards)
        {
            cb.push_CANport(&CANPorts);
        }
        for (canport &cp : CANPorts)
        {
            // std::thread(&canport::send, &cp);
            cp.puch_motor(&Motors);
        }
        ROS_INFO("\033[1;32mThe robot has %d motors\033[0m", Motors.size());

        // for (motor m:Motors)
        // {
        //     std::cout<<m.get_motor_belong_canboard()<<" "<<m.get_motor_belong_canport()<<" "<<m.get_motor_id()<<std::endl;
        // }
    }
    void enable_send()
    {
        for (canport& cp : CANPorts)
        {
            cp.enable_send();
        }
        // ROS_INFO("robot ok");
    }
    ~robot() {}
};
#endif