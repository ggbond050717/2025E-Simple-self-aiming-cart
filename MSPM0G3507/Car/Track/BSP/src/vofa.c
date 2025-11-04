#include "vofa.h"

#define MAX_CHANNEL 8

#define BYTE0(dwTemp)       (*(char *)(dwTemp))
#define BYTE1(dwTemp)       (*((char *)(dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(dwTemp) + 3))


float *UserData[MAX_CHANNEL]={0};//only transmit float
unsigned char Data_Number = 8;



void Upper_Computer_Init(float* addr)
{
    if(Data_Number < MAX_CHANNEL)UserData[Data_Number++]=addr;
}

unsigned char data_to_send[4*MAX_CHANNEL+4] = {0};
void Upper_Computer_Show_Wave(void)
{
	unsigned char cnt = 0;
    UserData[0]=&current_speeed1;
    UserData[1]=&current_speeed2;
    UserData[2]=&target_speed1;
	UserData[3]=&target_speed2;
	UserData[4]=&pwm_value1;
	UserData[5]=&pwm_value2;
  
	UserData[7]=&pid_output;
	for(int i=0;i<Data_Number;i++)
	{
		data_to_send[cnt++] = BYTE0(UserData[i]);
		data_to_send[cnt++] = BYTE1(UserData[i]);
		data_to_send[cnt++] = BYTE2(UserData[i]);
		data_to_send[cnt++] = BYTE3(UserData[i]);
	}

	data_to_send[cnt++] = 0x00;
	data_to_send[cnt++] = 0x00;
	data_to_send[cnt++] = 0x80;
	data_to_send[cnt++] = 0x7F;
   for(int i=0;i<4*MAX_CHANNEL+4;i++)
   {
	 while( DL_UART_isBusy(UART_0_INST) == true ) continue;
     DL_UART_Main_transmitData(UART_0_INST,(uint8_t)data_to_send[i]);
   }
   
	//----------------end----------------------
}


