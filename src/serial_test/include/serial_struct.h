#ifndef _SERIAL_STRUCT_H_
#define _SERIAL_STRUCT_H_
#include <stdint.h>
#include "motor_struct.h"
#include "serial/serial.h"
#include "ros/ros.h"
#include "crc/crc16.h"
/* struct */
#pragma pack(1)
typedef struct cdc_acm_tx_message_struct
{
    uint8_t head[2]; // 0xFD 0xFE
    motor_back_t motor_back;
    uint16_t crc16;
} cdc_acm_tx_message_t;

typedef struct cdc_acm_rx_message_struct
{
    uint8_t head[2]; // 0xFE 0xFD
    motor_cmd_t motor_cmd;
    uint16_t crc16;
} cdc_acm_rx_message_t;

#pragma pack()
/* class*/
class lively_serial
{
private:
    serial::Serial _ser;
    std::string *_port;
    std::string _result;
    uint32_t _baudrate;
    uint8_t _debug_level;
    bool init_flag, send_flag, recv_flag;
    cdc_acm_tx_message_t cdc_acm_tx_message; // 串口发送的信息，来自于Motor，反馈给PC
    cdc_acm_rx_message_t cdc_acm_rx_message; // 串口收到的信息，来自于PC，用于控制Motor
public:
    lively_serial(std::string *port, uint32_t baudrate, uint8_t debug_level)
        : _port(port), _baudrate(baudrate), _debug_level(debug_level)
    {
        init_flag = false;
        send_flag = false;
        recv_flag = false;
        _ser.setPort(*_port); // 设置打开的串口名称
        _ser.setBaudrate(_baudrate);
        serial::Timeout to = serial::Timeout::simpleTimeout(1000); // 创建timeout
        _ser.setTimeout(to);                                       // 设置串口的timeout
        // 打开串口
        try
        {
            _ser.open(); // 打开串口
        }
        catch (const std::exception &e)
        {
            ROS_ERROR_STREAM("Unable to open port "); // 打开串口失败，打印信息
        }
        if (_ser.isOpen())
        {
            ROS_INFO_STREAM("IMU Serial Port initialized."); // 成功打开串口，打印信息
        }
        else
        {
        }
        cdc_acm_rx_message.head[0] = 0xFE;
        cdc_acm_rx_message.head[1] = 0xFD;
        init_flag = true;
        // start receive thread
    }
    ~lively_serial();
    void send(uint8_t ID,int32_t position, int32_t velocity, int32_t torque, int16_t Kp, int16_t Kd);
    void recv();
};

/* variable */

/* function */

#endif