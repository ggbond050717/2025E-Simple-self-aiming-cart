#ifndef __PID_H
#define __PID_H

#include "bsp.h"
#include "encoder.h"
#include "track.h"
#include "bsp_tb6612.h"


extern uint8_t sensor_values[8];
extern uint8_t mode_flag;
typedef struct {
    float Kp;
    float Ki;
    float Kd;
    float error;
    float prevError;
    float prevPrevError;
    float integral;
    float derivative;
    float output;
    float maxOutput;
    float minOutput;
} PID_TypeDef;

extern PID_TypeDef track_pid;
extern PID_TypeDef motor1_pid;
extern PID_TypeDef motor2_pid;
extern PID_TypeDef angle_pid;
extern PID_TypeDef dis_pid;

void PID_Init(PID_TypeDef *pid, float Kp, float Ki, float Kd, float maxOutput, float minOutput);
float PID_Compute(PID_TypeDef *pid, float setpoint, float measurement);
float Inc_PID_Calculate(PID_TypeDef *pid, float setpoint, float feedback);
void Follow_Line(void);
void angle_line(float angle);
uint16_t abs_x(uint16_t x);
void pwm_calculate(void);


void pwm_break(void);        //急刹车


void dis_keep(void);
#endif
