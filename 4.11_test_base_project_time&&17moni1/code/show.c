#include "show.h"



uint8_t mode_lcd;//0 1
uint8_t auto_flag;//1为自动
float val;
float temp,temp_set=30;//温度
uint16_t v=200;//转速
uint16_t duty=20;
uint8_t over_flag;//0超速

void uart_process(void)
{
	if(rx_buff[0]=='M'&&rx_buff[1]=='\0')
	{
		if(auto_flag)
		printf("Mode:AUTO\r\n");
		else 	printf("Mode:MANU\r\n");

	}
		else if(rx_buff[0]=='T'&&rx_buff[1]=='\0')
	{
	printf("Temp:%.1f\r\n",temp);

	}
	else if(rx_buff[0]=='S'&&rx_buff[1]=='\0')
	{
	printf("Speed:%drpm\r\n",v);
	}
	else 	printf("ERROR\r\n");

	
}

void task(void)
{
	val=get_vol(&hadc2);
	if(val>=3)temp=80;
	else if(val<=0.3)temp=0;
	else temp=((80.0/2.7)*val-(80.0/9.0));
	if(auto_flag)
	{
		if(temp>=temp_set)
		{
			v=(uint16_t)(temp-temp_set)*20;
			if(v<100)v=100;if(v>1000)v=1000;
		}		
	}
	else if(!auto_flag)
	{
		if(v<100)v=100;if(v>1000)v=1000;
	}
	duty=v/10;
	TIM17->CCR1=duty;
	
	if(mode_lcd==0)
	{
		show_led(1,1);
		show_led(2,0);

	}
	if(mode_lcd)
	{
	show_led(1,0);
			show_led(2,1);

	}	
if(auto_flag)	show_led(3,1);else show_led(3,0);
	
		if(v>=800)
	{
		over_flag=1;
		show_led(8,led8state);
	}
	else
	{
		over_flag=0;
		led8state=0;
		show_led(8,led8state);
	}	
	
	
	show_lcd();
}

char text[20];


float get_vol(ADC_HandleTypeDef *hadc)
{
	HAL_ADC_Start(hadc);
	uint16_t temp=HAL_ADC_GetValue(hadc);
	return 3.3*temp/4096;
}


void show_lcd(void)
{
	if(mode_lcd==0)
	{
		sprintf(text,"        DATA    ");
		LCD_DisplayStringLine(Line1,(u8 *)text);
		if(auto_flag)
		{
		sprintf(text,"      MODE:AUTO    ");
		LCD_DisplayStringLine(Line3,(u8 *)text);		
		}
		else
		{
		sprintf(text,"      MODE:MANU    ");
		LCD_DisplayStringLine(Line3,(u8 *)text);		
		}	
		sprintf(text,"      TEMP:%.1f    ",temp);
		LCD_DisplayStringLine(Line4,(u8 *)text);
		sprintf(text,"      SPEED:%d    ",v);
		LCD_DisplayStringLine(Line5,(u8 *)text);		
	}
		if(mode_lcd==1)
	{
		sprintf(text,"        SET    ");
		LCD_DisplayStringLine(Line1,(u8 *)text);
		sprintf(text,"      TH:%.0f    ",temp_set);
		LCD_DisplayStringLine(Line3,(u8 *)text);
				sprintf(text,"                      ");
		LCD_DisplayStringLine(Line4,(u8 *)text);
				sprintf(text,"                      ");
		LCD_DisplayStringLine(Line5,(u8 *)text);
	
	}


}

/////////////////////led
void show_led(uint8_t led,uint8_t state)
{
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
	if(state)HAL_GPIO_WritePin(GPIOC,0x0080<<led,GPIO_PIN_RESET);
	else HAL_GPIO_WritePin(GPIOC,0x0080<<led,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
}


//===============================key
volatile uint8_t key_short,key_long;
static uint8_t long_state=1;
static uint16_t key_time;


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
			long_state=1;key_time=0;
		}
		if(last_state==i&&now_state==i)
		{
			if(++key_time==40&&long_state==1)
			{
				long_state=0;key_long=i;
			}			
		}
		if(last_state==i&&now_state==0)
		{
			if(long_state)key_short=i;
			else long_state=1;
		}
	}
	
	
}








