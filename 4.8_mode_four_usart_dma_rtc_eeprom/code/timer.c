#include "timer.h"



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static uint32_t count_20ms,count_1s;
	if(htim->Instance==TIM1)//1ms
	{
		if(++count_20ms==20)
		{
			count_20ms=0;key_loop();
		}
		if(++count_1s==1000)
		{
			count_1s=0;a++;
			//printf("Hello,World!  %d\r\n",a);
		}
	}
}

extern uint8_t rx_buff[20];
	extern DMA_HandleTypeDef hdma_usart1_rx;
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if(huart->Instance==USART1)
	{
		
		if(rx_buff[0]=='a'&&rx_buff[1]=='b'&&rx_buff[2]=='\0')
		{
			printf("yes\r\n");
		}else printf("no\r\n");
	// Size 参数就是硬件自动帮你数出来的：这帧数据到底收了多少字节
	HAL_UARTEx_ReceiveToIdle_DMA(huart,rx_buff,20);
	__HAL_DMA_DISABLE_IT(&hdma_usart1_rx,DMA_IT_HT);
	}
}




