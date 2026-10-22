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
		}
	}
}

