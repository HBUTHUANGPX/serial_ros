#include "../include/livelybot_serial.h"
unsigned int livelybot_serial::LD_SetLatencyTimer(unsigned short ms_time)
{
    return FT_SetLatencyTimer(Handle, ms_time);
}
unsigned int livelybot_serial::LD_read(void *buffer, int size, unsigned int *returnSize)
{

    return FT_Read(Handle, buffer, size, returnSize);
}
unsigned int livelybot_serial::LD_write(void *buffer, int size)
{
    return 1;
}
unsigned int livelybot_serial::LD_SetBaudRate(unsigned int baudrate)
{
    return FT_SetBaudRate(Handle, baudrate);
}
unsigned int livelybot_serial::LD_SetTimeouts(unsigned int dwReadTimeout, unsigned int dwWriteTimeout)
{
    return FT_SetTimeouts(Handle, dwReadTimeout, dwWriteTimeout); // infinite timeouts
}
unsigned int livelybot_serial::LD_ResetDevice()
{
    return FT_ResetDevice(Handle);
}
unsigned int livelybot_serial::LD_Open(void *name)
{
    return FT_OpenEx(name, FT_OPEN_BY_SERIAL_NUMBER, &Handle);
}
unsigned int livelybot_serial::LD_Close()
{
    return FT_Close(Handle);
}
