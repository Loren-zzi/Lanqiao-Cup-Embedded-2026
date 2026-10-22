#include "show.h"

int a,b;

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
void task(void)
{
	show_lcd();
	show_led(1,1);
	HAL_RTC_GetTime(&hrtc,&sTime,RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc,&sDate,RTC_FORMAT_BIN);
	

}

//===============================LCD
char text[20];
void show_lcd(void)
{
	sprintf(text,"%d",sTime.Seconds);
	LCD_DisplayStringLine(Line0,(u8 *)text);
	
	sprintf(text,"%d",b);
	LCD_DisplayStringLine(Line1,(u8 *)text);
}

//=======================================LED
void show_led(uint8_t led,uint8_t state)	
{
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
	if(state)HAL_GPIO_WritePin(GPIOC,0x0080<<led,GPIO_PIN_RESET);
	else HAL_GPIO_WritePin(GPIOC,0x0080<<led,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
}

//===============================================KEY
volatile uint8_t key_short,key_long;
static uint8_t key_time=0;
static uint8_t long_state=1;

uint8_t key_state()
{	
	if(!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0))return 1;
	if(!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1))return 2;
	if(!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2))return 3;
	if(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0))return 4;
	return 0;
}

void key_loop(void)
{
	static uint8_t last_state,now_state;
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
				long_state=0;
			}
		}
		if(last_state==i&&now_state==0)
		{
			if(long_state)key_short=i;
			else long_state=1;
		}
	}
}




