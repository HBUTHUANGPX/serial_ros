#ifndef _MOTOR_H_
#define _MOTOR_H_
#include "serial/serial.h"
#include "../serial_struct.h"
class motor
{
private:
    int type, id,CANport_num,CANboard_num;
    ros::NodeHandle n;
    lively_serial *ser;
public:
    motor(int _motor_num, int _CANport_num, int _CANboard_num,lively_serial *_ser):ser(_ser),CANport_num(_CANport_num),CANboard_num(_CANboard_num)
    {
        if (n.getParam("robot/CANboard/No_"+std::to_string(_CANboard_num)+
                       "_CANboard/CANport/CANport_"+std::to_string(_CANport_num)+
                       "/motor/motor"+std::to_string(_motor_num)+"/id",id))
            {ROS_INFO("Got params id: %d",id);}
        else
            {ROS_ERROR("Faile to get params");}
        if (n.getParam("robot/CANboard/No_"+std::to_string(_CANboard_num)+
                       "_CANboard/CANport/CANport_"+std::to_string(_CANport_num)+
                       "/motor/motor"+std::to_string(_motor_num)+"/type",type))
            {ROS_INFO("Got params type: %d",type);}
        else
            {ROS_ERROR("Faile to get params");}
    }
    ~motor(){}
    void send(int32_t position, int32_t velocity, int32_t torque, int16_t Kp, int16_t Kd)
    {
        ser->send(id, position, velocity, torque, Kp, Kd);
    }
    int get_motor_id(){return id;}
    int get_motor_type(){return type;}
    int get_motor_belong_canport(){return CANport_num;}
    int get_motor_belong_canboard(){return CANboard_num;}
};
#endif