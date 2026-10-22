#include "led_key.h"


//=================================================led
void show_led(uint8_t led,uint8_t state)
{
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
	if(state)HAL_GPIO_WritePin(GPIOC,0x0080<<led,GPIO_PIN_RESET);
	else HAL_GPIO_WritePin(GPIOC,0x0080<<led,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
}

//========================================================lcd
char text[20];
uint8_t highlight_line=0;
uint16_t adc_val;

double get_vol(ADC_HandleTypeDef *hadc);
void show_lcd(void)
{
	if(highlight_line==0)
	{
		  LCD_SetBackColor(Yellow);
		sprintf(text,"  hello,world!  ");
		LCD_DisplayStringLine(Line0,(u8*)text);
		  LCD_SetBackColor(Black);
		sprintf(text,"    count1:  %-3d",count1);
		LCD_DisplayStringLine(Line1,(u8*)text);
		sprintf(text,"    count2:  %-3d",count2);
		LCD_DisplayStringLine(Line2,(u8*)text);

	}
		if(highlight_line==1)
	{
		  
		sprintf(text,"  hello,world!  ");
		LCD_DisplayStringLine(Line0,(u8*)text);
		  LCD_SetBackColor(Yellow);
		sprintf(text,"    count1:  %-3d",count1);
		LCD_DisplayStringLine(Line1,(u8*)text);
		  LCD_SetBackColor(Black); 
		sprintf(text,"    count2:  %-3d",count2);
		LCD_DisplayStringLine(Line2,(u8*)text);
	}
			if(highlight_line==2)
	{
		  
		sprintf(text,"  hello,world!  ");		  
		LCD_DisplayStringLine(Line0,(u8*)text);		 
		sprintf(text,"    count1:  %-3d",count1);
		LCD_DisplayStringLine(Line1,(u8*)text);
    	  LCD_SetBackColor(Yellow);
		sprintf(text,"    count2:  %-3d",count2);
		LCD_DisplayStringLine(Line2,(u8*)text);
		  LCD_SetBackColor(Black);
	}
			sprintf(text,"    fre:%5d    ",fre);
		LCD_DisplayStringLine(Line4,(u8*)text);
	
				sprintf(text,"    fre2:%5d    ",fre2);
		LCD_DisplayStringLine(Line6,(u8*)text);
	
		sprintf(text,"    adc_val:%5.2f    ",get_vol(&hadc1));
		LCD_DisplayStringLine(Line8,(u8*)text);
}

double get_vol(ADC_HandleTypeDef *hadc)
{
	HAL_ADC_Start(hadc);
	adc_val= HAL_ADC_GetValue(hadc);
	return 3.3*adc_val/4096;
}


//===========================================================key
// 定义全局变量时加上 volatile
volatile uint8_t key_short;   
volatile uint8_t key_long;

static uint8_t key_time=0;
static uint8_t long_state=1;

uint8_t key_state(void)
{
	if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)==0) return 1;
	if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)==0) return 2;
	if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)==0) return 3;
	if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==0) return 4;
	
	return 0;
}

void key_loop(void)
{
	static uint8_t last_state=0,now_state=0;
	
	last_state=now_state;
	now_state=key_state();
	
	uint8_t i;
	for(i=1;i<=4;i++)
	{
		if(last_state==0&&now_state==i)
		{
			key_time=0;
			long_state=1;    //重置长按标志位
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


















