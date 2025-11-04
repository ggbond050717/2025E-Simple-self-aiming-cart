#include "hw_lcd.h"
#include "LCD.h"


void Lcd_Proc(void)
{
   
     // LCD_ShowSignedIntNum(0,0,sensor_values[0], 2, WHITE,BLACK,24);
     // LCD_ShowSignedIntNum(24,0,sensor_values[1], 2, WHITE,BLACK,24);
     // LCD_ShowSignedIntNum(48,0,sensor_values[2], 2, WHITE,BLACK,24);
     // LCD_ShowSignedIntNum(72,0,sensor_values[3], 2, WHITE,BLACK,24);
     // LCD_ShowSignedIntNum(96,0,sensor_values[4], 2, WHITE,BLACK,24);
     // LCD_ShowSignedIntNum(120,0,sensor_values[5], 2, WHITE,BLACK,24);
     // LCD_ShowSignedIntNum(144,0,sensor_values[6],2, WHITE,BLACK,24);
     // LCD_ShowSignedIntNum(168,0,sensor_values[7], 2, WHITE,BLACK,24);

    // LCD_ShowSignedIntNum(0,0,current_x, 5, WHITE,BLACK,24);
    // LCD_ShowSignedIntNum(144,0,current_y, 5, WHITE,BLACK,24);
    // LCD_ShowSignedIntNum(0,24,target_x, 5, WHITE,BLACK,24);
    // LCD_ShowSignedIntNum(144,24,target_y, 5, WHITE,BLACK,24);
    // LCD_ShowSignedIntNum(0,48,target_y, 5, WHITE,BLACK,24);
    // LCD_ShowSignedIntNum(0,72,parsed_number, 6, WHITE,BLACK,24);
//      LCD_ShowSignedIntNum(0,48,uart_parsed_nums[0], 5, WHITE,BLACK,24);
//     LCD_ShowSignedIntNum(144,48,uart_parsed_nums[1], 5, WHITE,BLACK,24);

         LCD_ShowSignedIntNum(0,96,key_mode, 4, WHITE,BLACK,24);
         LCD_ShowSignedIntNum(48,96,num1, 4, WHITE,BLACK,24);
         LCD_ShowSignedIntNum(96,96,stop_flag1, 4, WHITE,BLACK,24);
          LCD_ShowSignedIntNum(144,96,fx_flag, 4, WHITE,BLACK,24);
         LCD_ShowSignedIntNum(0,120,output, 5, WHITE,BLACK,24);
         LCD_ShowSignedIntNum(96,120,output2, 5, WHITE,BLACK,24);
        //  LCD_ShowSignedIntNum(144,96,start_flag, 4, WHITE,BLACK,24);
        //   LCD_ShowSignedIntNum(0,120,rmp_car, 4, WHITE,BLACK,24);
        //  LCD_ShowSignedIntNum(48,120,num1, 4, WHITE,BLACK,24);
   
         LCD_BLK_Set();//打开背光
}