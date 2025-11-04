#include "BJDJ.h"
#include "Emm_V5.h"
#include "math.h"
//位置模式
void PID_Init1(PID_TypeDef1 *pid, float Kp, float Ki, float Kd, float maxOutput, float minOutput)
{
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
    pid->error = 0;
    pid->prevError = 0;
	  pid->prevPrevError=0;
    pid->integral = 0;
    pid->derivative = 0;
    pid->output = 0;
    pid->maxOutput = maxOutput;
    pid->minOutput = minOutput;
}

float PID_Compute1(PID_TypeDef1 *pid, float setpoint, float measurement)
{
    /* 计算误差 */
	  
    pid->error = setpoint - measurement;
    
    /* 计算积分项 */
    pid->integral += pid->error;
    
    /* 积分限幅 */
    if (pid->integral > 100) pid->integral = 100;
    if (pid->integral < -100) pid->integral = -100;
    
    /* 计算微分项 */
    pid->derivative = (pid->error - pid->prevError) ;
    pid->prevError = pid->error;
    
    /* 计算PID输出 */
    pid->output = pid->Kp * pid->error + pid->Ki * pid->integral + pid->Kd * pid->derivative;
    
    /* 输出限幅 */
    if (pid->output > pid->maxOutput) pid->output = pid->maxOutput;
    if (pid->output < pid->minOutput) pid->output = pid->minOutput;
    
    return pid->output;	
}

void turn_line1(int pulse)
{
  if(pulse>0)
  {
    Emm_V5_Pos_Control_1(1, 1, 100, 5, pulse, 0, 1);
  }else{
    pulse=-pulse;
    Emm_V5_Pos_Control_1(1, 0, 100, 5, pulse, 0, 1);
  }
  
  
}
void turn_line2(int pulse)
{
    if(pulse>0)
  {
    Emm_V5_Pos_Control_1(2, 1, 100, 5, pulse, 0, 1);
  }else{
    pulse=-pulse;
    Emm_V5_Pos_Control_1(2, 0,100, 5, pulse, 0, 1);
  }
}
//速度模式
void turn_speed1(int vel)
{
  if(vel>0)
  {
    Emm_V5_Vel_Control(1, 1, vel, 0, 1);
  }else{
    vel=-vel;
    Emm_V5_Vel_Control(1, 0, vel,0,1);
  }
  
  
}
void turn_speed2(int vel)
{
    if(vel>0)
  {
    Emm_V5_Vel_Control(2, 1, vel, 0, 1);
  }else{
    vel=-vel;
    Emm_V5_Vel_Control(2, 0, vel, 0, 1);
  }
}
void Line_Proc(void)
{
        x_e1 = uart_parsed_nums[0] - 160;
        y_e1 = uart_parsed_nums[1] - 120;
        if(x_e1<0)
        {
            x_e1=-x_e1;
            Emm_V5_Pos_Control_2(2, 0, 5, 0, x_e1*10, 0, 1);
            delay_ms(10);
        }
       else {
            
            Emm_V5_Pos_Control_2(2, 1, 5, 0, x_e1*10, 0, 1);
            delay_ms(10);
       }
       delay_ms(10);
        if(y_e1<0)
        {
            y_e1=-y_e1;
            Emm_V5_Pos_Control_1(1, 1, 5, 0, y_e1*10, 0, 1);
            delay_ms(10);
        }
       else {
            
            Emm_V5_Pos_Control_1(1, 0, 5, 0, y_e1*10, 0,1);
            delay_ms(10);
       }
        delay_ms(10);
        Emm_V5_Synchronous_motion(0);        //同步运动
        delay_ms(10);

        if((uart_parsed_nums[1]>118&&uart_parsed_nums[1]<122 )&& (158<uart_parsed_nums[0]&&uart_parsed_nums[0]<162))
        {
         DL_GPIO_setPins(GPIOB, DL_GPIO_PIN_5);
        }
        else {
         DL_GPIO_clearPins(GPIOB, DL_GPIO_PIN_5);
        }
}

void diansai_3(void)
{
    if(uart_parsed_nums[1]==0 && uart_parsed_nums[0]==0 )
    {
      if(fx_flag)Emm_V5_Vel_Control(2, 1, 14,0,0);     //顺时针扫描
      else Emm_V5_Vel_Control(2, 0, 14,0,0);           //逆时针扫描
      delay_ms(10);
    } 
    else {
    Line_Proc();
    }
    
}

void Line_Proc_pid(void)
{
    current_x = uart_parsed_nums[0];
    target_x  = 160;
    y_e1 = uart_parsed_nums[1] - 120;
    float correction2 = PID_Compute1(&m2_pid,target_x, current_x);          //只对x轴进行kp
    output2 = correction2;
    
    if(y_e1<0)
    {
      y_e1=-y_e1;
      Emm_V5_Pos_Control_1(1, 1, 5, 0, y_e1*5, 0, 1);
      delay_ms(10);
    }
    else {    
      Emm_V5_Pos_Control_1(1, 0, 5, 0, y_e1*5, 0,1);
      delay_ms(10);
    }
   
    if(output2<0)
    {
      output2=-output2;
      if(zhuang_flag==1)          //开始转角
      {
       
        if(zhuang_num==1)                                     //分转角对步进电机加速补偿
        Emm_V5_Pos_Control_2(2, 1, 8, 0, output2+400, 0, 1);
        else if(zhuang_num==2)
        Emm_V5_Pos_Control_2(2, 1, 8, 0, output2+800, 0, 1);
        else if(zhuang_num==3)
        Emm_V5_Pos_Control_2(2, 1, 8, 0, output2+800, 0, 1);
        else if(zhuang_num==4)
        Emm_V5_Pos_Control_2(2, 1, 8, 0, output2+800, 0, 1);     
        delay_ms(10);
      }
      else {
      Emm_V5_Pos_Control_2(2, 1, 10, 0, output2, 0,1);
      delay_ms(10);
     }
    }
    else {
      if(zhuang_flag==1)
      {
        if (zhuang_num==1)
        Emm_V5_Pos_Control_2(2, 1, 8, 0, output2+400, 0, 1);    //逆时针跑，都是顺时针补偿
        else if (zhuang_num==2)
        Emm_V5_Pos_Control_2(2, 1, 8, 0, output2+400, 0, 1); 
        else if (zhuang_num==3)
        Emm_V5_Pos_Control_2(2, 1, 8, 0, output2+800, 0, 1); 
        else if (zhuang_num==4)
        Emm_V5_Pos_Control_2(2, 1, 8, 0, output2+800, 0, 1); 
        delay_ms(10);
      }else{
         Emm_V5_Pos_Control_2(2, 0, 10, 0, output2, 0,1);           
         delay_ms(10);
      }
    }
        
        Emm_V5_Synchronous_motion(0);
        delay_ms(10);

        if((uart_parsed_nums[1]>118&&uart_parsed_nums[1]<122 )&& (158<uart_parsed_nums[0]&&uart_parsed_nums[0]<162))
        {
          DL_GPIO_setPins(GPIOB, DL_GPIO_PIN_5);         //开激光
        }

}


