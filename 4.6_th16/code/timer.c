#include "timer.h"

uint16_t sec,min,hour;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static uint32_t count_20ms,count_1s;

	if(htim->Instance==TIM1)//1ms
	{
		if(++count_20ms==20)
		{
			count_20ms=0;key_loop();
		}
		if(time_star_flag==0)
		{//time_star_flag=1;
			if(++count_1s==1000)
		{
			count_1s=0;			
			if(++sec==60)
			{
				sec=0;
				if(++min==60)
				{
					min=0;hour++;
					if(hour>=24)hour=0;
				}
				
			}
		}
		}
	
	}
}
uint32_t cap_cal,fre_ic;
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM2)
	{
		cap_cal=TIM2->CCR1;
		TIM2->CNT=0;
		fre_ic=80000000/(80*cap_cal);
	}
}
