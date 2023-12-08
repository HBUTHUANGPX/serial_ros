#ifndef _LIVELYBOT_SERIAL_H_
#define _LIVELYBOT_SERIAL_H_
#include "../include/ftd2xx.h"
class livelybot_serial
{
private:
	FT_HANDLE Handle;
    
public:
    livelybot_serial(void *name,unsigned int baudrate)
    {

    }
    livelybot_serial()
    {

    }
    ~livelybot_serial()
    {

    }
    unsigned int 
    LD_SetLatencyTimer(unsigned short ms_time);
    unsigned int 
    LD_read(void *buffer,int size,unsigned int *returnSize);
    unsigned int 
    LD_write(void *buffer,int size);
    unsigned int 
    LD_SetBaudRate(unsigned int baudrate);
    unsigned int 
    LD_SetTimeouts(unsigned int ReadTimeout, unsigned int WriteTimeout);
    unsigned int 
    LD_ResetDevice();
    unsigned int 
    LD_Open(void *name);
    unsigned int 
    LD_Close();
};
#endif