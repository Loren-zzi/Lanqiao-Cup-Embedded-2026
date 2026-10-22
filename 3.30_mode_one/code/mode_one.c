#include "mode_one.h"



char text[20];
uint8_t highlight;
int  count;
void show_lcd(void)
{
	LCD_SetBackColor(highlight==0 ? Yellow:Black);
	sprintf(text,"    Hello,World!");
	LCD_DisplayStringLine(Line0,(u8*)text);
	
	LCD_SetBackColor(highlight==1 ? Yellow:Black);
	sprintf(text,"    count:  %3d",count);
	LCD_DisplayStringLine(Line1,(u8*)text);
	
	LCD_SetBackColor(Black);
}


//=========================================================
void show_led(uint8_t led,uint8_t state)
{             
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
   if(state)
		HAL_GPIO_WritePin(GPIOC,0x0080<<led,GPIO_PIN_RESET);
   else 
		HAL_GPIO_WritePin(GPIOC,0x0080<<led,GPIO_PIN_SET);
   HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
}


//===============================key
volatile uint8_t key_long;
volatile uint8_t key_short;

static uint8_t key_time;
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

//===============================================================
uint8_t led8_state;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static uint16_t count_1s,count_20ms;
	if(htim->Instance==TIM3)//1ms
	{
		if(++count_1s==1000)
		{
			count_1s=0;
			led8_state++;
			led8_state=led8_state%2;
		}
		if(++count_20ms==20)
		{
			count_20ms=0;
			key_loop();
		}
	}
}

