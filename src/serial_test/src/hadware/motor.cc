#include "../../include/hardware/motor.h"
template <typename T>
inline T motor::float2int(float in_data, uint8_t type)
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

inline float motor::int2float(int32_t in_data, uint8_t type)
{
    switch (type)
    {
    case 0: // radian float pos/vel to int32
        return (float)(in_data * my_2pi / 100000.0);
    case 1: // angle float pos/vel to int32
        return (float)(in_data * 360.0 / 100000.0);
    case 2: // float torque to int32
        return (float)(in_data / 100000.0);
    default:
        return float();
    }
}

void motor::fresh_cmd(float position, float velocity, float torque, float Kp, float Kd)
{
    cmd.motor_cmd.position = float2int<int32_t>(position, 1);
    cmd.motor_cmd.velocity = float2int<int32_t>(velocity, 1);
    cmd.motor_cmd.torque = float2int<int32_t>(torque, 2);
    cmd.motor_cmd.Kp = float2int<int16_t>(Kp, 3);
    cmd.motor_cmd.Kd = float2int<int16_t>(Kp, 3);
    cmd.crc16 = crc_ccitt(0x0000, (const uint8_t *)&cmd, sizeof(cdc_acm_rx_message_t) - 2);
    // ROS_INFO("CRC: 0x%x",cmd.crc16);
}

void motor::fresh_data(int32_t position, int32_t velocity, int32_t torque)
{
    p_msg.pos = data.position = int2float(position, 1);
    p_msg.vel = data.velocity = int2float(velocity, 1);
    p_msg.tau = data.torque = int2float(torque, 2);
    _motor_pub.publish(p_msg);
}