#include "bsp_tb6612.h"


//pwm控制，正负控制方向
void pwm_ctrol(int speed1,int speed2)
{
  if(speed1>0)
  {
       DL_GPIO_clearPins(AIN1_PORT, AIN1_A_PIN_24_PIN);     
    DL_GPIO_setPins(AIN2_PORT, AIN2_A_PIN_25_PIN); 
  }else{
    if(speed1<-120) speed1=-120;
    speed1=-speed1;
    DL_GPIO_setPins(AIN1_PORT, AIN1_A_PIN_24_PIN);        
    DL_GPIO_clearPins(AIN2_PORT, AIN2_A_PIN_25_PIN);    
  }
  
  if(speed2>0)
  {
     DL_GPIO_setPins(BIN1_PORT, BIN1_B_PIN_24_PIN);  
    DL_GPIO_clearPins(BIN2_PORT,BIN2_B_PIN_25_PIN); 
  }else{
    if(speed2<-120)  speed2=-120;
    speed2=-speed2;
    DL_GPIO_clearPins(BIN1_PORT, BIN1_B_PIN_24_PIN);  
    DL_GPIO_setPins(BIN2_PORT,BIN2_B_PIN_25_PIN); 

  }

  if(speed1>120) speed1=120;
  if(speed2>120) speed2=120;

  DL_TimerG_setCaptureCompareValue(PWM_0_INST, speed2, GPIO_PWM_0_C1_IDX);   //右电机(speed1,定时器A，通道1，PA16)
  DL_TimerG_setCaptureCompareValue(PWM_0_INST, speed1, GPIO_PWM_0_C0_IDX);   //左电机(speed2,定时器A，通道0，PA17)
}

