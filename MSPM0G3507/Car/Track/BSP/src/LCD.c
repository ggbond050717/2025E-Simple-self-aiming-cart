#include "hw_lcd.h"
#include "LCD.h"


void Lcd_Proc(void)
{
   if(lcd_flag==0)
   {
     LCD_ShowSignedIntNum(0,0,sensor_values[0], 2, WHITE,BLACK,24);
     LCD_ShowSignedIntNum(24,0,sensor_values[1], 2, WHITE,BLACK,24);
     LCD_ShowSignedIntNum(48,0,sensor_values[2], 2, WHITE,BLACK,24);
     LCD_ShowSignedIntNum(72,0,sensor_values[3], 2, WHITE,BLACK,24);
     LCD_ShowSignedIntNum(96,0,sensor_values[4], 2, WHITE,BLACK,24);
     LCD_ShowSignedIntNum(120,0,sensor_values[5], 2, WHITE,BLACK,24);
     LCD_ShowSignedIntNum(144,0,sensor_values[6],2, WHITE,BLACK,24);
     LCD_ShowSignedIntNum(168,0,sensor_values[7], 2, WHITE,BLACK,24);

     LCD_ShowSignedIntNum(0,24,Anolog[0], 5, WHITE,BLACK,24);
     LCD_ShowSignedIntNum(72,24,Anolog[1], 5, WHITE,BLACK,24);
     LCD_ShowSignedIntNum(144,24,Anolog[2], 5, WHITE,BLACK,24);
     LCD_ShowSignedIntNum(218,24,Anolog[3], 5, WHITE,BLACK,24);
     LCD_ShowSignedIntNum(0,48,Anolog[4], 5, WHITE,BLACK,24);
     LCD_ShowSignedIntNum(72,48,Anolog[5], 5, WHITE,BLACK,24);
     LCD_ShowSignedIntNum(144,48,Anolog[6],5, WHITE,BLACK,24);
     LCD_ShowSignedIntNum(218,48,Anolog[7], 5, WHITE,BLACK,24);

   }else{

      LCD_ShowSignedIntNum(0,0,black[0], 5, WHITE,BLACK,24);
      LCD_ShowSignedIntNum(72,0,black[1], 5, WHITE,BLACK,24);
      LCD_ShowSignedIntNum(144,0,black[2], 5, WHITE,BLACK,24);
      LCD_ShowSignedIntNum(218,0,black[3], 5, WHITE,BLACK,24);
      LCD_ShowSignedIntNum(0,24,black[4], 5, WHITE,BLACK,24);
      LCD_ShowSignedIntNum(72,24,black[5], 5, WHITE,BLACK,24);
      LCD_ShowSignedIntNum(144,24,black[6], 5, WHITE,BLACK,24);
      LCD_ShowSignedIntNum(218,24,black[7], 5, WHITE,BLACK,24);

      LCD_ShowSignedIntNum(0,72,white[0], 5, WHITE,BLACK,24);
      LCD_ShowSignedIntNum(72,72,white[1], 5, WHITE,BLACK,24);
      LCD_ShowSignedIntNum(144,72,white[2], 5, WHITE,BLACK,24);
      LCD_ShowSignedIntNum(218,72,white[3], 5, WHITE,BLACK,24);
      LCD_ShowSignedIntNum(0,96,white[4], 5, WHITE,BLACK,24);
      LCD_ShowSignedIntNum(72,96,white[5], 5, WHITE,BLACK,24);
      LCD_ShowSignedIntNum(144,96,white[6], 5, WHITE,BLACK,24);
      LCD_ShowSignedIntNum(218,96,white[7], 5, WHITE,BLACK,24);
   }
    

     LCD_ShowSignedIntNum(0,120,key_mode, 4, WHITE,BLACK,24);
     LCD_ShowSignedIntNum(48,120,num1, 4, WHITE,BLACK,24);
     LCD_ShowSignedIntNum(96,120,stop_flag1, 4, WHITE,BLACK,24);
     LCD_ShowSignedIntNum(144,120,fx_flag, 4, WHITE,BLACK,24);
     LCD_BLK_Set();//打开背光
}