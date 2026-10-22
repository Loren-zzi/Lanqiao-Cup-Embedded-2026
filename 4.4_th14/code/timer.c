#include "timer.h"


uint16_t b;
uint8_t change_fre_ok_flag;  //0代表已经转换到电平，1为转到高电平
uint8_t led2state;

uint32_t tar_fre=4000;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static uint32_t count_20ms,count_2s,count_5s,count_100ms;
	if(htim->Instance==TIM1)//1ms
	{
		if(++count_20ms==20)
		{
			count_20ms=0;
			key_loop();
		}
		if(++count_2s==2000)
		{
			count_2s=0;
			b+=2;
		}
		if(allow_change_flag)
		{


			if(++count_100ms==100)
			{
				count_100ms=0;
				led2state=!led2state;
//				if(mode_fre==1&&TIM2->PSC>=99&&TIM2->PSC<=199)//199->99
//				{
//					TIM2->PSC=TIM2->PSC-2;
//					if(TIM2->PSC<=99)
//					{
//						TIM2->PSC=99;change_fre_ok_flag=1;
//					}
//				}
//				
//				if(mode_fre==0&&TIM2->PSC>=99&&TIM2->PSC<=199)//99->199
//				{
//					TIM2->PSC=TIM2->PSC+2;
//					if(TIM2->PSC>=199)
//					{
//					TIM2->PSC=199;change_fre_ok_flag=0;
//					}
//				}
				//50次，每次
				if(mode_fre==1&&tar_fre>=4000&&tar_fre<=8000)//199->99  fre 4k-》8K 50次每次
				{
					tar_fre+=80;if(tar_fre>=80000){tar_fre=8000;change_fre_ok_flag=1;}
				}
				if(mode_fre==0&&tar_fre>=4000&&tar_fre<=8000)//
				{
					tar_fre-=80;if(tar_fre<=40000){tar_fre=4000;change_fre_ok_flag=0;}
				}
					TIM2->PSC=800000/tar_fre;								
			}
						if(++count_5s==5000)
			{
				count_5s=0;
				allow_change_flag=0;
				led2state=0;
			}
			
		}
	}
}



uint32_t cap_val;
uint32_t fre;
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM17)
	{
		cap_val=TIM17->CCR1;
		TIM17->CNT=0;
		fre=80000000/(80*cap_val);
	}

}














