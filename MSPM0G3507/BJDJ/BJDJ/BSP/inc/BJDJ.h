 #ifndef _BJDJ_H__
#define _BJDJ_H__

#include "board.h"
#include "bsp.h"

#define POSITION_TOLERANCE 2  // 位置误差容忍范围

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
} PID_TypeDef1;

void Line_Proc(void);
void turn_line1(int pulse);
void turn_line2(int pulse);

extern PID_TypeDef1 m1_pid;
extern PID_TypeDef1 m2_pid;
void PID_Init1(PID_TypeDef1 *pid, float Kp, float Ki, float Kd, float maxOutput, float minOutput);

float PID_Compute1(PID_TypeDef1 *pid, float setpoint, float measurement);

void Line_Proc_pid(void);

#endif
