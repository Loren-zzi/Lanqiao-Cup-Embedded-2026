#include "mode.h"

uint32_t fre,capture_val;
uint32_t Fre_NE555,capture_val2;
int16_t highlignt;
int16_t count;

double get_val(ADC_HandleTypeDef *hadc)
{
		HAL_ADC_Start(hadc);
	uint32_t adc_val=HAL_ADC_GetValue(hadc);
	return 3.3*adc_val/4096;
}

char text[20];
void show_lcd(void)
{
	LCD_SetBackColor(highlignt==0?Yellow:Black);
	sprintf(text,"    Hello,World!");
	LCD_DisplayStringLine(Line0,(u8 *)text);
	
	LCD_SetBackColor(highlignt== 1?Yellow:Black);
	sprintf(text,"    Count:%3d",a);
	LCD_DisplayStringLine(Line1,(u8 *)text);
	
//	LCD_SetBackColor(highlignt==2?Yellow:Black);
//	sprintf(text,"    Fre:%6d",fre);
//	LCD_DisplayStringLine(Line2,(u8 *)text);
//	
//	LCD_SetBackColor(highlignt==3?Yellow:Black);
//	sprintf(text,"    Fre2:%6d",Fre_NE555);
//	LCD_DisplayStringLine(Line3,(u8 *)text);
//	
//	LCD_SetBackColor(highlignt==4?Yellow:Black);
//	sprintf(text,"    ADC:%.2f",get_val(&hadc1));
//	LCD_DisplayStringLine(Line4,(u8 *)text);
	
	LCD_SetBackColor(Black);
}

void show_led(uint8_t led,uint8_t state)
{
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
	if(state)HAL_GPIO_WritePin(GPIOC,0x0080<<led,GPIO_PIN_RESET);
	else HAL_GPIO_WritePin(GPIOC,0x0080<<led,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);	
}

//===========================================================
volatile uint8_t key_short,key_long;
static uint8_t key_time,long_state=1;

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
			long_state=1;
			key_time=0;
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

uint8_t led8state;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static uint16_t count_1s,count_20ms;
	if(htim->Instance==TIM1)//1ms
	{
		if(++count_1s==1000)
		{
			count_1s=0;
			led8state++;
			led8state=led8state%2;			
		}
		if(++count_20ms==20)
		{
			count_20ms=0;
			key_loop();
		}
	}
}


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM17)
	{
		//capture_val=HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
		capture_val=TIM17->CCR1;
		TIM17->CNT=0;
		fre=80000000/(80*capture_val);
	}
	
	if(htim->Instance==TIM8)
	{
		//capture_val=HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
		capture_val2=TIM8->CCR1;
		TIM8->CNT=0;
		Fre_NE555=80000000/(80*capture_val2);
	}
}


uint8_t rec_count;
uint8_t rec_buff[20];
uint8_t rec_data;
uint8_t rec_flag;
char send_buff[20];
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)
	{
		TIM4->CNT=0;
		rec_flag=1;
		rec_buff[rec_count++]=rec_data;
		//HAL_UART_Transmit(huart,&rec_data,1,50);
		HAL_UART_Receive_IT(huart,&rec_data,1);
	}
}

void uart_rec(void)
{
	if(rec_flag)
	{
		if(TIM4->CNT>20)
		{
			if(rec_buff[0]=='l'&&rec_buff[1]=='q'&&rec_buff[2]=='b')
			{
				sprintf(send_buff,"yes\r\n");
				HAL_UART_Transmit(&huart1,(uint8_t *)send_buff,strlen(send_buff),50);
			}
			else 
			{
				sprintf(send_buff,"no\r\n");
				HAL_UART_Transmit(&huart1,(uint8_t *)send_buff,strlen(send_buff),50);
			}			
			rec_flag=0;
			rec_count=0;
			memset(rec_buff,0,sizeof(rec_buff));
		}
	}
}


