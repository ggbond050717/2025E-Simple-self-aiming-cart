
#ifndef	__ENCODER_H__
#define __ENCODER_H__

#include "board.h"
#include "stdio.h"
#include "string.h"
// 获得绝对值
#define ABS(a)      (a>0 ? a:(-a))

typedef struct{
    int Should_Get_Encoder_Count;   // 将要获得的编码器计数
    int Obtained_Get_Encoder_Count; // 得到的编码器的计数
}Encoder;

//均值滤波
#define MAX_SIZE 5  // 定义数组的固定长度,滤波长度
typedef struct {
    int data[MAX_SIZE];  // 固定大小的数组
    int size;           // 当前元素数量
} FixedArray;


extern FixedArray arr1;           //edcoder1
extern FixedArray arr2;           //encoder2
extern float rpm;
extern float distance1;
extern uint8_t sensor_values[8];
//滑动窗口均值滤波算法，动态滤波
void initArray(FixedArray *arr) ;
void append(FixedArray *arr, int value);
void removeFirst(FixedArray *arr);
int getAverage(const FixedArray *arr);

void Motor_Init(void);
int Motor_Get_Encoder(int dir);
void Motor_Set_PWM(int pwma,int pwmb);
void Motor_Stop(void);


void Encoder_Proc(void);

extern float distance;
extern uint8_t SR04_Flag; // 完成测量标志

#endif

