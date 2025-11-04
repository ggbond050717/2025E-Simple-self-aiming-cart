#include "pid.h"
#include "senser.h"

/* PID计算函数 */                  //位置式，计算目标速度,循迹误差增量
float PID_Compute(PID_TypeDef *pid, float setpoint, float measurement)
{
    pid->error = setpoint - measurement;
   
    //只用了kp
    pid->output = pid->Kp * pid->error ;
    
    if (pid->output > pid->maxOutput) pid->output = pid->maxOutput;
    if (pid->output < pid->minOutput) pid->output = pid->minOutput;
    
    return pid->output;
		
}
void PID_Init(PID_TypeDef *pid, float Kp, float Ki, float Kd, float maxOutput, float minOutput)
{
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
    pid->error = 0;
    pid->prevError = 0;
	pid->prevPrevError=0;           //上上次保留值
    pid->integral = 0;
    pid->derivative = 0;
    pid->output = 0;
    pid->maxOutput = maxOutput;
    pid->minOutput = minOutput;
}
//增量式pid,电机调pid
float Inc_PID_Calculate(PID_TypeDef *pid, float setpoint, float feedback)
{
    // 计算当前误差
    pid->error = setpoint - feedback;
    
    // 计算增量
    float delta = pid->Kp * (pid->error - pid->prevError)      // 比例项
                + pid->Ki * pid->error                        // 积分项
                + pid->Kd * (pid->error - 2 * pid->prevError + pid->prevPrevError); // 微分项
    
    // 保存前两次的误差(为下次计算做准备)
    pid->prevPrevError = pid->prevError;
    pid->prevError = pid->error;
    
    // 计算输出
    pid->output += delta;
    
    // 输出限幅
    if(pid->output > pid->maxOutput)
    {
        pid->output = pid->maxOutput;
    }
    else if(pid->output < pid->minOutput)
    {
        pid->output = pid->minOutput;
    }
    
    return pid->output;
}

static float prev_speed1 = 0.0f;
static float prev_speed2 = 0.0f;

#define FILTER_ALPHA 0.3f

void pwm_calculate(void)
{
    float temp_speed1 = Motor_Get_Encoder(0);
    float temp_speed2 = Motor_Get_Encoder(1);
    
    // 应用滤波，结果存回原变量名
    current_speeed1 = FILTER_ALPHA * temp_speed1 + (1 - FILTER_ALPHA) * prev_speed1;  //一阶滤波，速度平滑处理
    current_speeed2 = FILTER_ALPHA * temp_speed2 + (1 - FILTER_ALPHA) * prev_speed2;
    
    // 更新上一次的滤波结果
    prev_speed1 = current_speeed1;
    prev_speed2 = current_speeed2;
  if(ABS(target_speed1-current_speeed1)>2)              //>2
   {
     pwm_value1+=Inc_PID_Calculate(&motor1_pid,target_speed1,current_speeed1);
     if(pwm_value1>120) pwm_value1=120;
     else if(pwm_value1<-120)  pwm_value1=-120;
   }

   if(ABS(target_speed2-current_speeed2)>2)
   {
     pwm_value2+=Inc_PID_Calculate(&motor2_pid,target_speed2,current_speeed2);
     if(pwm_value2>120) pwm_value2=120;
     else if(pwm_value2<-120)  pwm_value2=-120;
   }
   pwm_ctrol(pwm_value1,pwm_value2);
}

void  Follow_Line(void)  //巡线
{
   
    float weight1[8]={-7,-5,-3,-1,1,3,5,7};
    int sum = 0;
    int count = 0;
    float position = 0;
    static float last_valid_position = 0;  // 静态变量保存上一个有效位置
    float correction = 0;
     for (int i = 0; i < 8; i++)
    {
        if (sensor_values[i])
        {
            sum += weight1[i];
            count++; 
        }
    }
     if (count > 0)
    {
        position = sum / count;
        last_valid_position = position;  // 更新上一个有效位置
     }
     else{
        position_flag=1;
     }
    if(position_flag)//进入直角
    {
        DL_GPIO_setPins(GPIOA, DL_GPIO_PIN_30);        //给云台转角信号
      
            target_speed2=19;    //转直角的速度，已调好
            target_speed1=9;
        if(sensor_values[4]==1&&sensor_values[3]==1)  
        {
            position_flag=0;                           //结束转角
            DL_GPIO_clearPins(GPIOA, DL_GPIO_PIN_30);
        }
        if(position_flag) return;
    }
   
    correction = PID_Compute(&track_pid,0, position);
    pid_output=correction;
    target_speed1 = base_speed - correction;
    target_speed2 = base_speed + correction;
}

//急刹
void pwm_break(void)
{
   target_speed1=target_speed2=0;
}

void TIMER_1_INST_IRQHandler(void)
{
	
	if( DL_TimerG_getPendingInterrupt(TIMER_1_INST) == DL_TIMER_IIDX_ZERO)
    {
       us3s_flag^=1;//lcd定时刷新
    }
     
}
