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
            if (_result.length() >= sizeof(cdc_acm_rx_message_struct))
            {
                if (_result[0]==0xFD && _result[1]==0xFE)
                {
                    for (size_t i = 0; i < sizeof(cdc_acm_rx_message_struct); i++)
                    {
                        printf("0x%02X ",_result[i]);
                    }
                    std::cout<<std::endl;                    
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

void lively_serial::send()
{
    send_flag = true;
}