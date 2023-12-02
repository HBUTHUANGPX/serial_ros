#ifndef _CANPORT_H_
#define _CANPORT_H_
#include "ros/ros.h"
#include "motor.h"
class canport
{
private:
    int motor_num;
    int CANport_num;
    ros::NodeHandle n;
    std::vector<motor> Motors;
public:
    canport(int _CANport_num,int _CANboard_num,lively_serial *ser)
    {
        if (n.getParam("robot/CANboard/No_"+
                        std::to_string(_CANboard_num)+
                        "_CANboard/CANport/CANport_"+std::to_string(_CANport_num)+"/motor_num",motor_num))
        {
            // ROS_INFO("Got params motor_num: %d",motor_num);
        }
        else
            {ROS_ERROR("Faile to get params motor_num");}
        for (size_t i = 1; i <= motor_num; i++)
        {
            Motors.push_back(motor(i,_CANport_num,_CANboard_num,ser));
        }
        
    }
    ~canport(){}
    void puch_motor(std::vector<motor> *_Motors)
    {
        for(motor m:Motors)
        {
            _Motors->push_back(m);
        }
    }
};
#endif