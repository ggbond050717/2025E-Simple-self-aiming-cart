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
#include "board.h"
#include "pid.h"
#include "bsp.h"
#include "hw_lcd.h"
#include "hw_spi.h"
#include "string.h"
#include "key.h"
#include "BJDJ.h"
#include "LCD.h"
#include "Emm_V5.h"

uint8_t first_flag=0;
//按键
uint8_t key_flag;
uint8_t key_val,key_down,key_up,key_old;

uint32_t us3s;

void diansai_3(void);
void Lcd_Proc(void);
void grey_proc(void);

//按键
uint8_t key_val,key_down,key_old,key_up;
uint8_t key_x;

//步进电机
PID_TypeDef1 m1_pid,m2_pid;
int pluse1;                    //脉冲值
int pluse2;       
float current_x,current_y;
float target_x,target_y;
int16_t output;                 //pid输出值查看
int16_t output2;

void end_proc(void);
void problem_2(void);
//整个按键逻辑
uint8_t key_mode;
uint8_t rmp_car=1;
uint8_t num1;            //转的圈数
uint8_t start_flag;
//串口接收
uint32_t uart_parsed_nums[MAX_NUM_COUNT] = {0};
volatile bool uart_data_ready = false;
uint8_t uart_num_count = 0;

//第三问反向控制标志位
uint8_t  fx_flag;
int x_e1;
int y_e1;

uint8_t  led_flag;
uint8_t zhuang_flag;
uint8_t zhuang_num;

int main(void)
{
    SYSCFG_DL_init();

    NVIC_ClearPendingIRQ(UART_1_INST_INT_IRQN);
    NVIC_EnableIRQ(UART_1_INST_INT_IRQN);

    NVIC_ClearPendingIRQ(TIMER_1_INST_INT_IRQN);
    NVIC_EnableIRQ(TIMER_1_INST_INT_IRQN);

	delay_ms(100);

    PID_Init1(&m1_pid, 1, 0, 0, 6400, -6400);//12
    PID_Init1(&m2_pid, 5, 0, 0, 6400, -6400);
    //LCD验证
    lcd_init();
    LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
    delay_ms(100);
    DL_GPIO_clearPins(GPIOB, DL_GPIO_PIN_5);
    while (1) {
    
    end_proc();

    if(DL_GPIO_readPins(GPIOA, DL_GPIO_PIN_29) != 0)        //转角信号
    {
         DL_GPIO_setPins(GPIOB,DL_GPIO_PIN_20);
        zhuang_flag=1;
        zhuang_num++;
        if(zhuang_num>5)  zhuang_num=1;
    }else{
        zhuang_flag=0;
         DL_GPIO_clearPins(GPIOB,DL_GPIO_PIN_20);
    }

    if(key_mode==0)  DL_GPIO_clearPins(GPIOB, DL_GPIO_PIN_22);

    if(key_mode==1)
    {
        DL_GPIO_setPins(GPIOB,DL_GPIO_PIN_22);
       if(uart_parsed_nums[0]!=0&&uart_parsed_nums[1]!=0)  Line_Proc();  
    }

    if(key_mode==2)
    {
        if(led_flag)  DL_GPIO_setPins(GPIOB, DL_GPIO_PIN_22);        //快闪为模式2
        else  DL_GPIO_clearPins(GPIOB, DL_GPIO_PIN_22);
        diansai_3();
    }

    if(key_mode==3)
    {
        if(led_flag)  DL_GPIO_setPins(GPIOB, DL_GPIO_PIN_22);        //慢闪为模式三
        else  DL_GPIO_clearPins(GPIOB, DL_GPIO_PIN_22);
        Line_Proc_pid();         //发挥pid
    }
   
    if(fx_flag)  DL_GPIO_setPins(GPIOB,DL_GPIO_PIN_20);
    else DL_GPIO_clearPins(GPIOB, DL_GPIO_PIN_20);
   
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
    }
}

void uart_clear_data_ready(void)
{
  uart_data_ready = false;
}


