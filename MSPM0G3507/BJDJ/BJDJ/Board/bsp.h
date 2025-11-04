#ifndef __BSP_H
#define __BSP_H
#include "board.h"

extern uint32_t us3s;

extern uint8_t key_val;
extern uint8_t key_down;
extern uint8_t key_old;
extern uint8_t key_up;
extern uint8_t position_flag;

extern int pluse1;
extern int pluse2;

extern float current_x;
extern float current_y;
extern float target_x;
extern float target_y;

extern int16_t output;
extern int16_t output2;
extern uint8_t stop_flag1;

extern uint8_t key_mode;
extern uint8_t rmp_car;
extern uint8_t num1;

//串口
#define MAX_NUM_COUNT 5    // 最多解析5个数字
#define MAX_DIGIT_COUNT 5  // 每个数字最大5位

// 全局变量声明
extern uint32_t uart_parsed_nums[MAX_NUM_COUNT];
extern volatile bool uart_data_ready;  // 中断中修改的变量需加volatile
extern uint8_t uart_num_count;

extern uint8_t fx_flag;

extern int x_e1;
extern int y_e1;

extern uint8_t  led_flag;
extern uint8_t zhuang_flag;
extern uint8_t zhuang_num;


#endif

