#include "encoder.h"
#include "bsp.h"
static volatile Encoder Encoder_A;
static volatile Encoder Encoder_B;
//滑动平均滤波
//初始化
void initArray(FixedArray *arr) {
    memset(arr->data, 0, sizeof(arr->data));
    arr->size = 0;
}
//加入新元素
void append(FixedArray *arr, int value) {
    arr->data[arr->size++] = value;
}
//删除第一个元素
void removeFirst(FixedArray *arr) {
    
    // 使用memmove将后面的元素前移
    memmove(arr->data, arr->data + 1, (arr->size - 1) * sizeof(int));
    arr->size--;
}
//返回均值
int getAverage(const FixedArray *arr) {
    int sum = 0;
    for (int i = 0; i < arr->size; i++) {
        sum += arr->data[i];
    }
    return sum / arr->size;
}

void Motor_Init(void)
{
	//编码器引脚外部中断

	NVIC_ClearPendingIRQ(ENCODER_INT_IRQN);
	NVIC_EnableIRQ(ENCODER_INT_IRQN);

    //定时器中断
	NVIC_ClearPendingIRQ(TIMER_0_INST_INT_IRQN);
	NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);
	NVIC_ClearPendingIRQ(TIMER_1_INST_INT_IRQN);
    NVIC_EnableIRQ(TIMER_1_INST_INT_IRQN);
	initArray(&arr1);
	initArray(&arr2);
}


int Motor_Get_Encoder(int dir)
{
	if( !dir )
		return getAverage(&arr1);        //返回均值

	return getAverage(&arr2);
	// if( !dir )
	// 	return Encoder_A.Obtained_Get_Encoder_Count;  //返回原始值

	// return Encoder_A.Obtained_Get_Encoder_Count;
}
//外部中断读取
void GROUP1_IRQHandler(void)
{
	uint32_t gpio_interrup = 0;

	gpio_interrup = DL_GPIO_getEnabledInterruptStatus(ENCODER_PORT,ENCODER_E1A_PIN|ENCODER_E1B_PIN|ENCODER_E2A_PIN|ENCODER_E2B_PIN);

    // encoderA
	if((gpio_interrup & ENCODER_E1A_PIN)==ENCODER_E1A_PIN)
	{
		if(!DL_GPIO_readPins(ENCODER_PORT,ENCODER_E1B_PIN))
		{
			Encoder_A.Should_Get_Encoder_Count--;
		}
		else
		{
			Encoder_A.Should_Get_Encoder_Count++;
		}
	}
	else if((gpio_interrup & ENCODER_E1B_PIN)==ENCODER_E1B_PIN)
	{
		if(!DL_GPIO_readPins(ENCODER_PORT,ENCODER_E1A_PIN))
		{
			Encoder_A.Should_Get_Encoder_Count++;
		}
		else
		{
			Encoder_A.Should_Get_Encoder_Count--;
		}
	}

	// encoderB
	if((gpio_interrup & ENCODER_E2A_PIN)==ENCODER_E2A_PIN)
	{
		if(!DL_GPIO_readPins(ENCODER_PORT,ENCODER_E2B_PIN))
		{
			Encoder_B.Should_Get_Encoder_Count--;
		}
		else
		{
			Encoder_B.Should_Get_Encoder_Count++;
		}
	}
	else if((gpio_interrup & ENCODER_E2B_PIN)==ENCODER_E2B_PIN)
	{
		if(!DL_GPIO_readPins(ENCODER_PORT,ENCODER_E2A_PIN))
		{
			Encoder_B.Should_Get_Encoder_Count++;
		}
		else
		{
			Encoder_B.Should_Get_Encoder_Count--;
		}
	}
	DL_GPIO_clearInterruptStatus(ENCODER_PORT,ENCODER_E1A_PIN|ENCODER_E1B_PIN|ENCODER_E2A_PIN|ENCODER_E2B_PIN);
}

//电机编码器脉冲计数
void TIMER_0_INST_IRQHandler(void)            //5ms读取一次
{
	if( DL_TimerG_getPendingInterrupt(TIMER_0_INST) == DL_TIMER_IIDX_ZERO )
	{
        // 两个电机安装相反，所以编码器值也要相反 
        Encoder_A.Obtained_Get_Encoder_Count =- Encoder_A.Should_Get_Encoder_Count*5;
        Encoder_B.Obtained_Get_Encoder_Count = Encoder_B.Should_Get_Encoder_Count*5;
		rpm=-Encoder_A.Should_Get_Encoder_Count;
		distance1+=rpm*10*0.0048/13;             //距离计算
        if(arr1.size<MAX_SIZE||arr2.size<MAX_SIZE) 
		{
			append(&arr1, Encoder_A.Obtained_Get_Encoder_Count);
			append(&arr2, Encoder_B.Obtained_Get_Encoder_Count);
		}else{
			removeFirst(&arr1);
			removeFirst(&arr2);
			append(&arr1, Encoder_A.Obtained_Get_Encoder_Count);
			append(&arr2, Encoder_B.Obtained_Get_Encoder_Count);
		}
        //编码器计数值清零 
        Encoder_A.Should_Get_Encoder_Count = 0;
        Encoder_B.Should_Get_Encoder_Count = 0;
	}
}