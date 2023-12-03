#include "../include/serial_struct.h"
lively_serial::~lively_serial()
{
}

void lively_serial::recv()
{
    ROS_INFO_STREAM("start thread");
    while (ros::ok() && init_flag)
    {
        // this method is read the avaliable from the buffer
        _result = _ser.read(_ser.available());
        if (_result.length() > 15)
        {
            // for (size_t i = 0; i < _result.length(); i++)
            // {
            //     printf("0x%02X ", *(uint8_t *)&_result[i]);
            // }
            // std::cout << std::endl;
            if (*(uint8_t *)&_result[0] == 0xFD && *(uint8_t *)&_result[1] == 0xFE)
            {

                    ROS_INFO_STREAM("");
                // printf("0x%04X\n ", *(uint16_t *)&_result[15]);
                // printf("0x%04X\n ", crc_ccitt(0x0000, (const uint8_t *)&_result[0], sizeof(cdc_acm_tx_message_t) - 2));

                if (*(uint16_t *)&_result[15] == crc_ccitt(0x0000, (const uint8_t *)&_result[0], sizeof(cdc_acm_tx_message_t) - 2))
                {
                    for (size_t i = 0; i < sizeof(cdc_acm_tx_message_t); i++)
                    {
                        printf("0x%02X ", *(uint8_t *)&_result[i]);
                    }
                    std::cout << std::endl;
                }
            }
            else
            {
                // ROS_INFO_STREAM("else 3");
            }
        }
        else
        {
            // ROS_INFO_STREAM("else 2");
            continue;
        }
        // TODO : this method is read one bytes
    }
}

void lively_serial::send(uint8_t ID, int32_t position, int32_t velocity, int32_t torque, int16_t Kp, int16_t Kd)
{
    // ROS_INFO_STREAM("START");
    cdc_acm_rx_message.motor_cmd.position = position;
    cdc_acm_rx_message.motor_cmd.velocity = velocity;
    cdc_acm_rx_message.motor_cmd.torque = torque;
    cdc_acm_rx_message.motor_cmd.Kp = Kp;
    cdc_acm_rx_message.motor_cmd.Kd = Kd;
    cdc_acm_rx_message.motor_cmd.ID = ID;
    cdc_acm_rx_message.crc16 = crc_ccitt(0x0000, (const uint8_t *)&cdc_acm_rx_message, sizeof(cdc_acm_rx_message_t) - 2);
    uint8_t *byte_ptr = (uint8_t *)&cdc_acm_rx_message;
    // ROS_INFO_STREAM("STEP1");
    // for (size_t i = 0; i < sizeof(cdc_acm_rx_message_t); i++)
    // {
    //     printf("0x%02X ", byte_ptr[i]);
    // }
    // std::cout << std::endl;
    // ROS_INFO_STREAM("STEP2");
    _ser.write((const uint8_t *)&cdc_acm_rx_message, sizeof(cdc_acm_rx_message));
    // ROS_INFO_STREAM("END"); // STEP2 -> END 1.7ms  START -> END 1.71
}
void lively_serial::send(cdc_acm_rx_message_t *_cdc_acm_rx_message)
{
    // uint8_t *byte_ptr = (uint8_t *)_cdc_acm_rx_message;
    // ROS_INFO_STREAM("STEP1");
    // for (size_t i = 0; i < sizeof(cdc_acm_rx_message_t); i++)
    // {
    //     printf("0x%02X ", byte_ptr[i]);
    // }
    // std::cout << std::endl;
    _ser.write((const uint8_t *)_cdc_acm_rx_message, sizeof(cdc_acm_rx_message_t));
}