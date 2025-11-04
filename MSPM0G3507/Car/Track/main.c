/*
 * Copyright (c) 2021, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "ti_msp_dl_config.h"
#include "bsp_tb6612.h"
#include "board.h"
#include "pid.h"
#include "track.h"
#include "bsp.h"
#include "vofa.h"
#include "hw_lcd.h"
#include "hw_spi.h"
#include "string.h"
#include "key.h"
#include "senser.h"
#include "LCD.h"


uint8_t sensor_values[8];          //循迹存储数组
//电机
uint16_t base_speed=22;             //循迹基础速度       //40  60

float current_speeed1;                //L
float current_speeed2;                //R
float target_speed1;                  //左轮目标速度
float target_speed2;                  //右轮目标速度
float pwm_value1=10;                  //pwm输出值
float pwm_value2=10;

//pid
PID_TypeDef track_pid,motor1_pid,motor2_pid;

FixedArray arr1;           //edcoder1
FixedArray arr2;           //encoder2
float distance1;           //直线测距
float rpm;                 //实际脉冲数
uint8_t car_flag;          //循迹状态 
float pid_output;          //pid计算输出，波形显示

//按键
uint8_t key_flag;
uint8_t key_val,key_down,key_up,key_old;

//定时器
uint32_t us3s;
uint8_t us3s_flag;

//按键
uint8_t key_val,key_down,key_old,key_up;
uint8_t key_x;
//grey
unsigned short Anolog[8]={0};
unsigned short white[8]={ 2600,2600,2600,2600,2600,2600,2600,2600};
unsigned short black[8]={ 2000,2000,2000,2000,2000,2000,2000,2000};
unsigned short Normal[8];
unsigned char Digtal;
No_MCU_Sensor sensor;

//循迹位置标志位
uint8_t position_flag;

//步进电机
int pluse1;                    //脉冲值
int pluse2;       
float current_x,current_y;
float target_x,target_y;
int16_t output;                 //pid输出值查看
int16_t output2;
uint8_t stop_flag1;             //停止标志位


//整个按键逻辑
uint8_t key_mode;
uint8_t rmp_car=1;
uint8_t num1;            //转的圈数
uint8_t start_flag;
//串口
uint32_t uart_parsed_nums[MAX_NUM_COUNT] = {0};
volatile bool uart_data_ready = false;
uint8_t uart_num_count = 0;

// 清除数据就绪标志
uint8_t  fx_flag;        //第三问反向控制
int x_e1;                //纯脉冲跟踪
int y_e1;
uint8_t lcd_flag;         //lcd切换屏幕。

void diansai_3(void);
void grey_proc(void);
void end_proc(void);

int main(void)
{
    SYSCFG_DL_init();
    Motor_Init();                             //编码电机初始化

    //灰度初始化
    No_MCU_Ganv_Sensor_Init_Frist(&sensor);
	No_Mcu_Ganv_Sensor_Task_Without_tick(&sensor);
	Get_Anolog_Value(&sensor,Anolog);
    No_MCU_Ganv_Sensor_Init(&sensor,white,black);
	
	delay_ms(100);

    PID_Init(&track_pid,0.25,0,0,100, -100);          //看情况调参数
  
    PID_Init(&motor1_pid, 0.04,0,0,100, -100); 
	PID_Init(&motor2_pid, 0.04,0,0,100, -100);
    //LCD验证
    lcd_init();
    LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
    delay_ms(100);
    DL_GPIO_clearPins(GPIOB, DL_GPIO_PIN_5);
    target_speed1 = base_speed;                      //上电给初速度
    target_speed2 = base_speed;
    while (1) {
    
    if(start_flag) DL_GPIO_setPins(GPIOB, DL_GPIO_PIN_22);   //启动标志位
    else DL_GPIO_clearPins(GPIOB, DL_GPIO_PIN_22);

    grey_proc();
    Read_Tracking_Sensors1(sensor_values);

    end_proc();
    if(key_mode==0) 
    {
        if(us3s_flag) Lcd_Proc();
    }

    if(key_mode==1)
    {
        if(start_flag)
        {
            if(distance1<44*num1)            //按距离停止
            {
                Follow_Line();
                pwm_calculate();
                delay_ms(1);
            }
            else
            {
                pwm_break();
                pwm_ctrol(0, 0);
            }
        }
        else
        {
            if(us3s_flag) Lcd_Proc();
            pwm_ctrol(0,0);
        }
    }

}
    
}
void end_proc(void)
{
    key_val=Key_Read();
	key_down=key_val&(key_val^key_old);
	key_up=~key_val&(key_val^key_old);
	key_old=key_val;
   
    switch(key_down)
    {
        
        case 1: key_mode++;if(key_mode>4) key_mode=0;
        break;

        case 2: num1++; if(num1>5)  num1=0;
        break;

        case 3:
        break;

        case 4:  start_flag^=1;  break;
        case 5:  fx_flag^=1;     break;

        case 6:
                for(int i=0;i<8;i++)                //脱机调黑色阈值
                {
                  black[i]=Anolog[i]+800;   
                }
        break;

        case 7:
                for(int j=0;j<8;j++)                //脱机调白色阈值
                {
                  white[j]=Anolog[j]-500;
                }                         
        break;

        case 8:lcd_flag^=1;LCD_Fill(0,0,LCD_W,LCD_H,BLACK);    //切屏
        break;

    }
}
void grey_proc(void)
{
   //无时基
   No_Mcu_Ganv_Sensor_Task_Without_tick(&sensor);
   //有时基
   //No_Mcu_Ganv_Sensor_Task_With_tick(&sensor)

   Digtal=Get_Digtal_For_User(&sensor);
   // lc_printf("Digtal %d-%d-%d-%d-%d-%d-%d-%d\r\n",(Digtal>>0)&0x01,(Digtal>>1)&0x01,(Digtal>>2)&0x01,(Digtal>>3)&0x01,(Digtal>>4)&0x01,(Digtal>>5)&0x01,(Digtal>>6)&0x01,(Digtal>>7)&0x01);
		
   if(Get_Anolog_Value(&sensor,Anolog)){
	// lc_printf("Anolog %d,%d,%d,%d,%d,%d,%d,%d\r\n",Anolog[0],Anolog[1],Anolog[2],Anolog[3],Anolog[4],Anolog[5],Anolog[6],Anolog[7]);
   }
			
   if(Get_Normalize_For_User(&sensor,Normal)){
			
   }
}

void uart_clear_data_ready(void)
{
    uart_data_ready = false;
}

