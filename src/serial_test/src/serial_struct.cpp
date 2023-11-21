#include "../include/serial_struct.h"
lively_serial::~lively_serial()
{
}

void lively_serial::recv()
{
    while (ros::ok() && init_flag)
    {
        if (send_flag)
        {
            // this method is read the avaliable from the buffer
            _result = _ser.read(_ser.available());
            if (_result.length() >= sizeof(cdc_acm_tx_message_t))
            {
                if (_result[0] == 0xFD && _result[1] == 0xFE)
                {
                    for (size_t i = 0; i < sizeof(cdc_acm_tx_message_t); i++)
                    {
                        printf("0x%02X ", _result[i]);
                    }
                    std::cout << std::endl;
                }
                recv_flag = true;
                send_flag = false;
            }
            else
            {
                continue;
            }
            // TODO : this method is read one bytes
        }
        else
        {
            continue;
        }
    }
}

void lively_serial::send(uint8_t ID,int32_t position, int32_t velocity, int32_t torque, int16_t Kp, int16_t Kd)
{
    cdc_acm_rx_message.motor_cmd.position = position;
    cdc_acm_rx_message.motor_cmd.velocity = velocity;
    cdc_acm_rx_message.motor_cmd.torque = torque;
    cdc_acm_rx_message.motor_cmd.Kp = Kp;
    cdc_acm_rx_message.motor_cmd.Kd = Kd;
    cdc_acm_rx_message.motor_cmd.ID = 0x01;
    cdc_acm_rx_message.crc16 = crc_ccitt(0x0000, (const uint8_t *)&cdc_acm_rx_message, sizeof(cdc_acm_rx_message_t) - 2);
    uint8_t *byte_ptr = (uint8_t *)&cdc_acm_rx_message;
    for (size_t i = 0; i < sizeof(cdc_acm_rx_message_t); i++)
    {
        printf("0x%02X ", byte_ptr[i]);
    }
    std::cout << std::endl;
    _ser.write((const uint8_t *)&cdc_acm_rx_message, sizeof(cdc_acm_rx_message));
    send_flag = true;
}