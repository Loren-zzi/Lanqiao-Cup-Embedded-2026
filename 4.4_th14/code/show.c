#include "show.h"
#include "math.h"

float MH,ML;

uint8_t allow_change_flag;//5s   0运行 1不允许
uint8_t change_RK_flag; //0为R 1为K；
uint8_t mode_lcd=0;//DATA
uint8_t mode_fre=0;//0为低频模式，1为高平模式

uint8_t adc_flag;//0 解锁 1 锁定

uint16_t pwm_count;

uint8_t R=1;
uint8_t K=1;

uint8_t change_R=1;
uint8_t change_K=1;


uint8_t led3state;

float V;

float adc_vol;
void task(void)
{
	if(mode_lcd==0)
	{
	show_led(1,1);
	}
		
	else 
	{
	show_led(1,0);
	}
	show_led(2,led2state);
show_led(3,led3state);
	
	if(adc_flag==0)
	{
		led3state=0;
	adc_vol=get_vol(&hadc2);
	if(adc_vol>=3)duty=85;
	else if(adc_vol<=1)duty=10;
	else duty=(float)((75.0/2.0)*adc_vol-27.5);
	}
	else
	{
		led3state=1;
	}
	
	TIM2->CCR2=duty;
	
	V=(float)(fre*2.0*3.14*R)/(100.0*K);
	static uint32_t V_start_time;
	static float last_V;
	
	if(fabs(V-last_V)>0.1)
	{
		last_V=V;
		V_start_time=HAL_GetTick();
	}
	else 
	{
		if(HAL_GetTick()-V_start_time>=2000)
		{
			if(mode_fre==0&&change_fre_ok_flag==0)
			{
				if(V>=ML)ML=V;
			}
			if(mode_fre==1&&change_fre_ok_flag==1)
			{
				if(V>=MH)MH=V;
			}	
		}
	}
	

	if(mode_lcd==2)
	{
		change_RK_flag=0;
		R=change_R;K=change_K;
	}
	
	show_lcd();
}
//=======================================================
float get_vol(ADC_HandleTypeDef *hadc)
{
	HAL_ADC_Start(hadc);
	float adc_vol=(float)HAL_ADC_GetValue(hadc);
	return (3.3*adc_vol)/4096.0;
}
//=================================================LCD
char text[20];
void show_lcd(void)
{	
	if(mode_lcd==0)
	{
	sprintf(text,"        DATA");
	LCD_DisplayStringLine(Line1,(uint8_t *)text);
					if(change_fre_ok_flag)
			{
						sprintf(text,"     M=H         ");
			LCD_DisplayStringLine(Line3,(uint8_t *)text);	
			}
			else
			{
			sprintf(text,"     M=L        ");
			LCD_DisplayStringLine(Line3,(uint8_t *)text);			
			}

	sprintf(text,"     P=%2.0f%%       ",duty);
	LCD_DisplayStringLine(Line4,(uint8_t *)text);
			
			sprintf(text,"     V=%.1f     ",V);
	LCD_DisplayStringLine(Line5,(uint8_t *)text);


	}
	if(mode_lcd==1)
	{
			sprintf(text,"        PARA");
	LCD_DisplayStringLine(Line1,(uint8_t *)text);
	sprintf(text,"     R=%d      ",change_R);
	LCD_DisplayStringLine(Line3,(uint8_t *)text);
	sprintf(text,"     K=%d      ",change_K);
	LCD_DisplayStringLine(Line4,(uint8_t *)text);
		sprintf(text,"                ");
	LCD_DisplayStringLine(Line5,(uint8_t *)text);
	}
		if(mode_lcd==2)
	{
			sprintf(text,"        RECD");
	LCD_DisplayStringLine(Line1,(uint8_t *)text);
	sprintf(text,"     N=%d        ",pwm_count);
	LCD_DisplayStringLine(Line3,(uint8_t *)text);
	sprintf(text,"     MH=%.1f",MH);
	LCD_DisplayStringLine(Line4,(uint8_t *)text);
	sprintf(text,"     ML=%.1f",ML);
	LCD_DisplayStringLine(Line5,(uint8_t *)text);		
	}
	
}


//=====================================LCD
volatile uint8_t key_short,key_long;
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
			if(++key_time==100&&long_state==1)
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

//====================================================LED
void show_led(uint8_t led,uint8_t state)
{
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
	if(state)HAL_GPIO_WritePin(GPIOC,0x0080<<led,GPIO_PIN_RESET);
	else HAL_GPIO_WritePin(GPIOC,0x0080<<led,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
}



