#include "timer.h"

uint16_t time;
uint8_t led2state;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static uint16_t count_20ms,count_1s;
	if(htim->Instance==TIM2)
	{
		if(++count_20ms>=20)
		{
			count_20ms=0;
			key_loop();
		}
		if(++count_1s==1000)
		{
			count_1s=0;
			time++;
			led2state++;
			led2state=(led2state)%2;
		}
	}
}

uint8_t rec_data,count0;
uint8_t rec_flag;
uint8_t rec_buff[20];
char send_buff[20];

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)
	{
		TIM4->CNT=0;
		//HAL_UART_Transmit(huart,&rec_data,1,50);
		rec_flag=1;
		rec_buff[count0++]=rec_data;
		HAL_UART_Receive_IT(huart,&rec_data,1);		
	}
}

void uart_date_rec(void)
{
	if(rec_flag)
	{
		if(TIM4->CNT>=20)
		{
			if(rec_buff[0]=='a'&&rec_buff[1]=='b')
			{
				sprintf(send_buff,"abc");
				HAL_UART_Transmit(&huart1,(uint8_t *)send_buff,sizeof(send_buff),50);
			}
			else
			{
				sprintf(send_buff,"error");
				HAL_UART_Transmit(&huart1,(uint8_t *)send_buff,sizeof(send_buff),50);
			}
			//        // --- 核心清零操作 ---
            // 重置状态
            rec_flag=0;
            count0=0;
            memset(rec_buff, 0, 20); 
			
		}
	}
}




