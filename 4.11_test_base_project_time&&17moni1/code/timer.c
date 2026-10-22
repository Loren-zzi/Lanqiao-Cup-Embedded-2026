#include "timer.h"



uint8_t led8state;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static uint32_t count_20ms,count_1s,count_100ms;
	if(htim->Instance==TIM1)
	{
		if(++count_20ms==20)
		{
			count_20ms=0;key_loop();
		}
//		if(++count_1s==1000)
//		{
//			count_1s=0;
//			//printf("go\r\n");
//		}
		if(over_flag)
		{
			if(++count_100ms==100)
		{
			count_100ms=0;
			led8state=!led8state;
		}
		}

	}
}


