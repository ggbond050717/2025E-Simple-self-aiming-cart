#include "pid.h"

void TIMER_1_INST_IRQHandler(void)
{
	
	if( DL_TimerG_getPendingInterrupt(TIMER_1_INST) == DL_TIMER_IIDX_ZERO)
    {
      us3s++;
      if(key_mode==2)  led_flag^=1;

      if(key_mode==3&&us3s>5)
     {
        us3s=0;
        led_flag^=1;
     }   
    }
}

void UART_1_INST_IRQHandler(void)         //解析k230数据
{
    static uint8_t Rx_mode = 0;           // 状态机状态
    static uint8_t digit_count = 0;
    static uint32_t current_num = 0;
    static uint32_t nums[MAX_NUM_COUNT];  // 临时存储解析的数字
    static uint8_t num_index = 0;
    uint8_t Rx_data;

    switch(DL_UART_getPendingInterrupt(UART_1_INST))
    {
        case DL_UART_IIDX_RX:  // 接收中断
            Rx_data = DL_UART_Main_receiveData(UART_1_INST);
            
            switch(Rx_mode)
            {
                case 0:  // 等待第一个$
                    if(Rx_data == '$') Rx_mode = 1;
                    break;
                    
                case 1:  // 等待第二个$
                    if(Rx_data == '$') Rx_mode = 2;
                    else Rx_mode = 0;
                    break;
                    
                case 2:  // 等待C
                    if(Rx_data == 'C') {
                        Rx_mode = 3;
                        // 重置解析变量
                        digit_count = 0;
                        current_num = 0;
                        num_index = 0;
                        memset(nums, 0, sizeof(nums));
                    } else Rx_mode = 0;
                    break;
                    
                case 3:  // 等待第一个逗号
                    if(Rx_data == ',') Rx_mode = 4;
                    else Rx_mode = 0;
                    break;
                    
                case 4:  // 接收数据中
                    if(Rx_data >= '0' && Rx_data <= '9') {
                        if(digit_count < MAX_DIGIT_COUNT) {
                            current_num = current_num * 10 + (Rx_data - '0');
                            digit_count++;
                            Rx_mode = 7;
                        } else Rx_mode = 0;  // 超过最大位数
                    }
                    else if(Rx_data == '#') Rx_mode = 5;
                    else Rx_mode = 0;  // 无效字符
                    break;
                    
                case 5:  // 收到第一个#
                    if(Rx_data == '#') {
                        // 保存最后一个数字
                        if(digit_count > 0 && num_index < MAX_NUM_COUNT) {
                            nums[num_index++] = current_num;
                        }
                        // 复制到全局变量
                        memcpy(uart_parsed_nums, nums, sizeof(nums));
                        uart_num_count = num_index;
                        uart_data_ready = true;  // 标记数据就绪
                        Rx_mode = 0;  // 重置状态机
                    } else Rx_mode = 0;
                    break;
                    
                case 7:  // 接收数字中
                    if(Rx_data >= '0' && Rx_data <= '9') {
                        if(digit_count < MAX_DIGIT_COUNT) {
                            current_num = current_num * 10 + (Rx_data - '0');
                            digit_count++;
                        } else Rx_mode = 0;
                    }
                    else if(Rx_data == ',') {
                        if(num_index < MAX_NUM_COUNT) {
                            nums[num_index++] = current_num;
                            current_num = 0;
                            digit_count = 0;
                            Rx_mode = 4;
                        } else Rx_mode = 0;  // 超过最大数字数量
                    }
                    else if(Rx_data == '#') {
                        if(num_index < MAX_NUM_COUNT) {
                            nums[num_index++] = current_num;
                        }
                        current_num = 0;
                        digit_count = 0;
                        Rx_mode = 5;
                    }
                    else Rx_mode = 0;  // 无效字符
                    break;
                    
                default:
                    Rx_mode = 0;
                    break;
            }
            break;

        default:
         
            break;
    }

    NVIC_ClearPendingIRQ(UART_1_INST_INT_IRQN);
}
