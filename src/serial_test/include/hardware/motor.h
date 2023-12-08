#ifndef _MOTOR_H_
#define _MOTOR_H_
#include "../serial_struct.h"
#include <stdint.h>
#include "ros/ros.h"
#include "pai_msgs/MotorState.h"
#define my_2pi (6.28318530717)
#define my_pi (3.14159265358)
class motor
{
private:
    int type, id, CANport_num, CANboard_num;
    ros::NodeHandle n;
    // lively_serial *ser;
    motor_back_t data;
    ros::Publisher _motor_pub;
    pai_msgs::MotorState p_msg;

public:
    cdc_acm_rx_message_t cmd;
    motor(int _motor_num, int _CANport_num, int _CANboard_num) : CANport_num(_CANport_num), CANboard_num(_CANboard_num)
    {
        _motor_pub = n.advertise<pai_msgs::MotorState>("/" + std::to_string(_CANboard_num) + "_" + std::to_string(_CANport_num) + "_/"+std::to_string(_motor_num), 1);

        if (n.getParam("robot/CANboard/No_" + std::to_string(_CANboard_num) + "_CANboard/CANport/CANport_" + std::to_string(_CANport_num) + "/motor/motor" + std::to_string(_motor_num) + "/id", id))
        {
            // ROS_INFO("Got params id: %d",id);
        }
        else
        {
            ROS_ERROR("Faile to get params id");
        }
        if (n.getParam("robot/CANboard/No_" + std::to_string(_CANboard_num) + "_CANboard/CANport/CANport_" + std::to_string(_CANport_num) + "/motor/motor" + std::to_string(_motor_num) + "/type", type))
        {
            // ROS_INFO("Got params type: %d",type);
        }
        else
        {
            ROS_ERROR("Faile to get params type");
        }
        memset(&cmd,0,sizeof(cmd));
        cmd.motor_cmd.ID = id;
        cmd.head[0] = 0xFE;
        cmd.head[1] = 0xFD;
        data.ID = id;
    }
    ~motor() {}
    template <typename T>
    inline T float2int(float in_data, uint8_t type);
    inline float int2float(int32_t in_data, uint8_t type);
    void fresh_cmd(float position, float velocity, float torque, float Kp, float Kd);
    void fresh_data(int32_t position, int32_t velocity, int32_t torque);
    int get_motor_id() { return id; }
    int get_motor_type() { return type; }
    int get_motor_belong_canport() { return CANport_num; }
    int get_motor_belong_canboard() { return CANboard_num; }
    cdc_acm_rx_message_t* return_cmd_p(){return &cmd;}
    motor_back_t *get_current_motor_state(){return &data;}
};
#endif