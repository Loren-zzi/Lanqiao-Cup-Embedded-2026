#include "timer.h"



uint8_t led7_state;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static uint16_t count_1s,count_20ms;

	if(htim->Instance==TIM3)  //10ms
	{			
		if(++count_1s==100)
		{
			count_1s=0;
			led7_state++;
			led7_state=led7_state%2;
		}
		if(++count_20ms==2)
		{
			count_20ms=0;
			key_loop();
		}
		
	}

}

uint32_t fre,capture_val;
uint32_t fre2,capture_val2;
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM17)
	{
		capture_val=TIM17->CCR1;
		//capture_val=HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
		TIM17->CNT=0;
		fre=80000000/(80*capture_val);
	}
		
	if(htim->Instance==TIM16)
	{
		capture_val2=TIM16->CCR1;
		//capture_val2=HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
		TIM16->CNT=0;
		fre2=80000000/(80*capture_val2);
	}
	
}

uint8_t uart_count;
uint8_t  rec_date;
uint8_t rec_flag;
uint8_t rec_buff[20];
char send_buff[20];

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)
	{
	//	HAL_UART_Transmit(huart,&rec_date,1,50);
		TIM4->CNT=0;
		
		rec_flag=1;
		rec_buff[uart_count]=rec_date;
		uart_count++;
	
		HAL_UART_Receive_IT(huart,&rec_date,1);
		
		
	}

}

//char debug[20];


void uart_date_rec(void)
{
	if(rec_flag)
	{
//		sprintf(debug, "rec[1]=%d\r\n", rec_buff[0]);
//HAL_UART_Transmit(&huart1, (uint8_t*)debug, strlen(debug), 50);
		if(TIM4->CNT>=200)
	{
			if(rec_buff[0]=='a'&& rec_buff[1]=='b'&& rec_buff[2]=='c')
//		if(rec_buff[0]=='a')
		{
			sprintf(send_buff,"abc\r\n");
			HAL_UART_Transmit(&huart1,(uint8_t *)send_buff,strlen(send_buff),50);
		}
		else
		{
		sprintf(send_buff,"error\r\n");
	//printf("rec: %02x %02x %02x\n", rec_buff[0], rec_buff[1], rec_buff[2]);
		HAL_UART_Transmit(&huart1,(uint8_t *)send_buff,strlen(send_buff),50);
		}
		
//		rec_flag=0;
//		for(int i=0;i<uart_count;i++)
//		{
//			rec_buff[i]=0;
//		}
//		uart_count=0;
		        rec_flag = 0;    // 关闭开关，等待下次数据
        uart_count = 0;  // 计数清零
        memset(rec_buff, 0, sizeof(rec_buff)); // 清空缓冲区
	}
	}

}

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//if(huart->Instance==USART1)
//{
//	// 增加最小的缓冲区溢出检查
//	if(uart_count < 20) {
//		rec_buff[uart_count] = rec_date;
//		uart_count++;
//	}
//	
//	// 重置超时计数器并设置标志位
//	TIM4->CNT=0;
//	rec_flag=1;

//	HAL_UART_Receive_IT(huart,(uint8_t *)&rec_date,1);
//}
//}



//void uart_date_rec(void)
//{
//    if(rec_flag)
//    {
//        // 尝试增加超时时间以确保接收完整
//        if(TIM4->CNT > 500)
//        {
//            
//            if(uart_count >= 3 && rec_buff[0]=='a' && rec_buff[1]=='b' && rec_buff[2]=='c')
//            {
//                // 回显 OK
//                sprintf(send_buff,"abc\r\n");
//                HAL_UART_Transmit(&huart1,(uint8_t *)send_buff,strlen(send_buff),50);
//            }
//            else
//            {
//                sprintf(send_buff, "error\r\n");
//            HAL_UART_Transmit(&huart1, (uint8_t *)send_buff, strlen(send_buff), 50);
//                
//                // sprintf(send_buff,"error\r\n");
//                // HAL_UART_Transmit(&huart1,(uint8_t *)send_buff,strlen(send_buff),50);
//            }
//            
//            // 重置状态
//            rec_flag=0;
//            uart_count=0;
//            memset(rec_buff, 0, 20); 
//        }
//    }
//}
//void uart_date_rec(void)
//{
//    // 只有“收过数”且“距离最后一个字节过去了一段时间”才处理
//    // 这里的 500 取决于你的 TIM4 频率，建议对应物理时间 20ms-50ms
//    if(rec_flag == 1 && TIM4->CNT > 500) 
//    {
//        // 增加长度判定：确保至少收到了 3 个字节
//        if(uart_count >= 3 && rec_buff[0]=='a' && rec_buff[1]=='b' && rec_buff[2]=='c')
//        {
//            sprintf(send_buff, "abc\r\n");
//            HAL_UART_Transmit(&huart1, (uint8_t *)send_buff, strlen(send_buff), 50);
//        }
//        else
//        {
//            sprintf(send_buff, "error\r\n");
//            HAL_UART_Transmit(&huart1, (uint8_t *)send_buff, strlen(send_buff), 50);
//        }
//        
//        // --- 核心清零操作 ---
//        rec_flag = 0;    // 关闭开关，等待下次数据
//        uart_count = 0;  // 计数清零
       // memset(rec_buff, 0, sizeof(rec_buff)); // 清空缓冲区
//    }
//}


