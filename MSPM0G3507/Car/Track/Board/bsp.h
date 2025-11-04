#ifndef __BSP_H
#define __BSP_H
#include "board.h"





extern float target_speed1;
extern float target_speed2;
extern float current_speeed1;
extern float current_speeed2;
extern uint16_t base_speed;        

extern float distance1;  
extern float pwm_value1;
extern float pwm_value2;
extern uint8_t car_flag;




extern uint16_t base_speed2;


extern float pid_output;
extern uint32_t us3s;
extern uint8_t us3s_flag;


extern uint8_t key_val;
extern uint8_t key_down;
extern uint8_t key_old;
extern uint8_t key_up;
extern uint8_t key_x;

extern unsigned char Digtal;
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

extern uint16_t rmpx;
extern uint16_t rmpy;
extern uint8_t index_i;





extern uint8_t key_mode;
extern uint8_t rmp_car;
extern uint8_t num1;

extern uint8_t start_flag;
extern  uint8_t sensor_values[8];  


//串口
#define MAX_NUM_COUNT 5    // 最多解析5个数字
#define MAX_DIGIT_COUNT 5  // 每个数字最大5位

// 全局变量声明
extern uint32_t uart_parsed_nums[MAX_NUM_COUNT];
extern volatile bool uart_data_ready;  // 中断中修改的变量需加volatile
extern uint8_t uart_num_count;

extern uint8_t fx_flag;
extern uint8_t in_corner;
extern int x_e1;
extern int y_e1;

extern unsigned short Anolog[8];
extern unsigned short white[8];
extern unsigned short black[8];
extern uint8_t lcd_flag;
#endif
