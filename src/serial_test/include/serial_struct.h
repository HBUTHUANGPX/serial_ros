#ifndef _SERIAL_STRUCT_H_
#define _SERIAL_STRUCT_H_
#include <stdint.h>
#include "motor_struct.h"
#include "serial/serial.h"
#include "ros/ros.h"
/* struct */
typedef struct cdc_acm_tx_message_struct
{
    uint8_t head[2];//0xFD 0xFE
    motor_cmd_struct motor_cmd;
    uint16_t crc16;
};

typedef struct cdc_acm_rx_message_struct
{
    uint8_t head[2];//0xFE 0xFD
    motor_back_struct motor_back;
    uint16_t crc16;
};
/* class*/
class lively_serial
{
private:
    serial::Serial _ser;
    const std::string *_port;
    std::string _result;
    uint32_t _baudrate;
    uint8_t _debug_level;
    bool init_flag, send_flag, recv_flag;

public:
    lively_serial(const std::string *port, uint32_t baudrate, uint8_t debug_level)
        : _port(port), _baudrate(baudrate), _debug_level(debug_level)
    {
        init_flag = false;
        send_flag = false;
        recv_flag = false;
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
        init_flag = true;
        // start receive thread
    }
    ~lively_serial();
    void send();
    void recv();
};

/* variable */

/* function */

#endif