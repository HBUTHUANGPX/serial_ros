#ifndef _MOTOR_STRUCT_H_
#define _MOTOR_STRUCT_H_
#include <stdint.h>
typedef struct motor_cmd_struct
{
    int32_t position;
    int32_t velocity;
    int32_t torque;
    int16_t Kp;
    int16_t Kd;
} motor_cmd_t;
typedef struct motor_back_struct
{
    int32_t position;
    int32_t velocity;
    int32_t torque;
} motor_back_t;
#endif