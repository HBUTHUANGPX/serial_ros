#ifndef _CANBOARD_H_
#define _CANBOARD_H_
#include <iostream>
#include <vector>
#include "canport.h"
#include "ros/ros.h"
class canboard
{
private:
    int CANport_num;
    ros::NodeHandle n;
    std::vector<canport> CANport;
    // std::vector<motor> motor;
public:
    canboard(int _CANboard_ID,lively_serial *ser)
    {
            if (n.getParam("robot/CANboard/No_"+
                            std::to_string(_CANboard_ID)+"_CANboard/CANport_num",CANport_num))
                {ROS_INFO("Got params CANport_num: %d",CANport_num);}
            else
                {ROS_ERROR("Faile to get params");}
            for (size_t j = 1; j <= CANport_num; j++)
            {
                CANport.push_back(canport(j,_CANboard_ID,ser));
            }
    }
    ~canboard(){}
    int get_CANport_num()
    {
        return CANport_num;
    }
    void push_CANport(std::vector<canport> *_CANport)
    {
        for (canport c:CANport)
        {
            _CANport->push_back(c);
        }
        
    }
};
#endif