#include "track.h"
#include "senser.h"
extern uint8_t sensor_values[8];
void Read_Tracking_Sensors1(uint8_t *sensor_values)
{
         for(uint8_t i=0;i<8;i++)
    {
        if((Digtal >> i)&0x01)  
            sensor_values[i] = 0;
        else
            sensor_values[i] = 1;
    }
        
}

void Track_printf(uint8_t *sensor_values)
{
    for(u8 i=0;i<8;i++)
    {
        lc_printf("sensor_values[%d] =%d\r\n", i,sensor_values[i]);
    }
    delay_ms(10);
}



