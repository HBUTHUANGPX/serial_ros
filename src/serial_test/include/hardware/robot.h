#ifndef _ROBOT_H_
#define _ROBOT_H_
#include <iostream>
#include "canboard.h"
#include "ros/ros.h"
class robot
{
private:
    std::string robot_name,Serial_Type,CAN_type,CANboard_type;
    int arm_dof,leg_dof,CANboard_num;
    ros::NodeHandle n;
    std::vector<canboard> CANboards;
    std::vector<canport> CANports;
    std::vector<motor> Motors;
public:
    robot(lively_serial *ser){
        if (n.getParam("robot/CANboard_num",CANboard_num))
            {ROS_INFO("Got params CANboard_num: %d",CANboard_num);}
        else
            {ROS_ERROR("Faile to get params");}
        for (size_t i = 1; i <= CANboard_num; i++)
        {
            CANboards.push_back(canboard(i,ser));
        }
        for (canboard cb:CANboards)
        {
            cb.push_CANport(&CANports);
        }
        for (canport cp:CANports)
        {
            cp.puch_motor(&Motors);
        }
        for (motor m:Motors)
        {
            std::cout<<m.get_motor_belong_canboard()<<" "<<m.get_motor_belong_canport()<<" "<<m.get_motor_id()<<std::endl;
        }
    }
    ~robot(){}
};
#endif