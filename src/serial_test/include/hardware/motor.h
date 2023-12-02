#ifndef _MOTOR_H_
#define _MOTOR_H_
#include "serial/serial.h"
#include "../serial_struct.h"
#define my_2pi (6.28318530717)
#define my_pi (3.14159265358)
class motor
{
private:
    int type, id, CANport_num, CANboard_num;
    ros::NodeHandle n;
    lively_serial *ser;
    cdc_acm_rx_message_t cmd;

public:
    motor(int _motor_num, int _CANport_num, int _CANboard_num, lively_serial *_ser) : ser(_ser), CANport_num(_CANport_num), CANboard_num(_CANboard_num)
    {
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
        cmd.motor_cmd.ID = id;
        cmd.head[0] = 0xFE;
        cmd.head[1] = 0xFD;
    }
    ~motor() {}
    template <typename T>
    T float2int(float in_data, uint8_t type)
    {
        switch (type)
        {
        case 0: // radian float pos/vel to int32
            return (int32_t)in_data / my_2pi * 100000;
        case 1: // angle float pos/vel to int32
            return (int32_t)in_data / 360 * 100000;
        case 2: // float torque to int32
            return (int32_t)in_data * 100000;
        case 3: // float kp/kd to int32
            return (int16_t)in_data * 32767;
        default:
            return T();
        }
    }
    void send(float position, float velocity, float torque, float Kp, float Kd)
    {
        ser->send(id, float2int<int32_t>(position, 1),
                  float2int<int32_t>(velocity, 1),
                  float2int<int32_t>(torque, 2),
                  float2int<int16_t>(Kp, 3),
                  float2int<int16_t>(Kp, 3));
    }
    /*无参数send 需要先调用fresh_cmd,然后调用此函数发送*/
    void send()
    {
        ser->send(&cmd);
    }
    void fresh_cmd(float position, float velocity, float torque, float Kp, float Kd)
    {
        cmd.motor_cmd.position = float2int<int32_t>(position, 1);
        cmd.motor_cmd.velocity = float2int<int32_t>(velocity, 1);
        cmd.motor_cmd.torque = float2int<int32_t>(torque, 2);
        cmd.motor_cmd.Kp = float2int<int16_t>(Kp, 3);
        cmd.motor_cmd.Kd = float2int<int16_t>(Kp, 3);
        cmd.crc16 = crc_ccitt(0x0000, (const uint8_t *)&cmd, sizeof(cdc_acm_rx_message_t) - 2);
    }
    int get_motor_id() { return id; }
    int get_motor_type() { return type; }
    int get_motor_belong_canport() { return CANport_num; }
    int get_motor_belong_canboard() { return CANboard_num; }
};
#endif