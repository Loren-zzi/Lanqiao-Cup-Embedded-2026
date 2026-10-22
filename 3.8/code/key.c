#include "key.h"


volatile uint8_t key_long=0;
volatile uint8_t key_short;

static uint8_t key_time=0;
static uint8_t long_state=1;


uint8_t key_state(void)
{
	if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)==0)return 1;
	if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)==0)return 2;
	if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)==0)return 3;
	if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==0)return 4;
	return 0;
}

void key_loop(void)
{
	static uint8_t last_state=0,now_state=0;
	last_state=now_state;
	now_state=key_state();
	
	for(uint8_t i=1;i<=4;i++)
	{
		if(last_state==0&&now_state==i)
		{
			key_time=0;
			long_state=1;
		}
		if(last_state==i&&now_state==i)
		{
			if(++key_time==40&&long_state==1)
			{
				key_long=i;
				long_state=0;//锁住
			}
		}
		if(last_state==i&&now_state==0)
		{
			if(long_state==1)
			{
				key_short=i;
			}
			else
			{
				long_state=1;
			}

		}
	}

}


