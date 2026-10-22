#include "show.h"
#include "math.h"
int a,b;

uint8_t time_star_flag;//0重启

uint8_t mode_canshu;//0123
uint8_t mode_lcd;// 0 1 2
uint8_t lock_flag;//0解锁   1锁定
//========================
uint16_t DS=1;
uint16_t DR=80;
uint16_t n_D;//duty的n
uint16_t real_dn;
uint16_t duty,test_duty;
float VA;
//===============================
uint16_t FS=100;
uint16_t FR=2000;
uint16_t n_F;
uint16_t real_fn;
uint16_t out_fre,test_out_fre;
float VB;
float test,test1,test2;

float vol_a,vol_b;
uint32_t CF,DF;
uint16_t CD;
int16_t XF;

uint8_t odd_flag;//0正常  1异常
uint32_t odd_cf,odd_cd,odd_xf,odd_h,odd_m,odd_s,odd_df;
 uint16_t test_DS,test_DR,test_FS,test_FR;
void task(void)
{
	vol_b=get_vol(&hadc1);
	vol_a=get_vol(&hadc2);
	DF=fre_ic;
	if(DF)
	{
		CF=800000/(TIM17->PSC+1);
		CD=duty;		
	}		XF=fabs((float)CF-(float)DF);
	if(XF>=1000&&odd_flag==0)
	{
		//show_led(3,1);
		odd_flag=1;odd_cf=CF;odd_cd=CD;odd_xf=XF;odd_h=hour;odd_m=min;odd_s=sec;odd_df=	DF;
	}
	
	if(odd_flag)show_led(3,1);  else show_led(3,0);
	                                                                                                                                                                                                                                                                         if(XF<1000&&odd_flag==1)odd_flag=0;
	if(lock_flag==0)
	{
		show_led(2,0);
		n_D=(DR-10)/DS;VA=3.3/(n_D+1);
		real_dn=vol_a/(float)VA;duty=(10+DS*real_dn);
		if(duty>=100)duty=100;else if(duty<=11)duty=11;
		//test=vol_a;test1=VA;test2=real_dn;
		
		n_F=(FR-1000)/FS;VB=(3.3/(n_F+1));
		real_fn=vol_b/(float)VB;out_fre=(1000+FS*real_fn);
		if(out_fre<=1010)out_fre=1010;
		(TIM17->PSC)=800000/out_fre-1;
		//test=real_fn;test1=VB;test2=vol_b;
	}
	else  show_led(2,1);
	
	if(mode_lcd==0)
	{
		show_led(1,1);
	mode_canshu=0;
		if((test_DS<test_DR)&&(test_FS<test_FR)&&((test_DR-10)>=test_DS)&&((test_FR-1000)>=test_FS)&&(test_DR<=90))
		{
			DS=test_DS;FS=test_FS;DR=test_DR;FR=test_FR;
		}
		
	}else show_led(1,0);
	
	if(mode_lcd==1)
	{
	test_DS=DS;test_DR=DR;test_FS=FS;test_FR=FR;
	}
	if(mode_lcd==2)
	{
		
	}
	
	show_lcd();

}


float get_vol(ADC_HandleTypeDef *hadc)
{
	HAL_ADC_Start(hadc);
	float vol=HAL_ADC_GetValue(hadc);
	return 3.3*vol/4096;
}

//===============================LCD
char text[21];
void show_lcd(void)
{
	if(mode_lcd==0)
	{
		sprintf(text,"       PWM           ");
	LCD_DisplayStringLine(Line1,(u8 *)text);		
		sprintf(text,"   CF=%dHz          ",CF);
	LCD_DisplayStringLine(Line3,(u8 *)text);
		sprintf(text,"   CD=%d%%        ",CD);
	LCD_DisplayStringLine(Line4,(u8 *)text);
		sprintf(text,"   DF=%dHz              ",DF);
	LCD_DisplayStringLine(Line5,(u8 *)text);
if(lock_flag)		
		sprintf(text,"   ST=LOCK            ");
else sprintf(text,"   ST=UNLOCK        ");
	LCD_DisplayStringLine(Line6,(u8 *)text);
		sprintf(text,"   %.2dH%.2dM%.2dS          ",hour,min,sec);
	LCD_DisplayStringLine(Line7,(u8 *)text);		

//		sprintf(text,"%.3f  %.3f   %.3f   ",test,test1,test2);
//	LCD_DisplayStringLine(Line9,(u8 *)text);
	}
	
		if(mode_lcd==1)
	{
		sprintf(text,"       RECD           ");
	LCD_DisplayStringLine(Line1,(u8 *)text);
		
		sprintf(text,"   CF=%dHz             ",odd_cf);
	LCD_DisplayStringLine(Line3,(u8 *)text);
		sprintf(text,"   CD=%d%%           ",odd_cd);
	LCD_DisplayStringLine(Line4,(u8 *)text);
		sprintf(text,"   DF=%dHz              ",odd_df);
	LCD_DisplayStringLine(Line5,(u8 *)text);		
		sprintf(text,"   XF=%d            ",odd_xf);
	LCD_DisplayStringLine(Line6,(u8 *)text);
		sprintf(text,"   %.2dH%.2dM%.2dS",odd_h,odd_m,odd_s);
	LCD_DisplayStringLine(Line7,(u8 *)text);	

		
	}

		if(mode_lcd==2)
	{
		sprintf(text,"       PARA           ");
	LCD_DisplayStringLine(Line1,(u8 *)text);
		
		sprintf(text,"   DS=%d%%         ",test_DS);
	LCD_DisplayStringLine(Line3,(u8 *)text);
		sprintf(text,"   DR=%d%%         ",test_DR);
	LCD_DisplayStringLine(Line4,(u8 *)text);
		sprintf(text,"   FS=%dHz               ",test_FS);
	LCD_DisplayStringLine(Line5,(u8 *)text);		
		sprintf(text,"   FR=%dHz            ",test_FR);
	LCD_DisplayStringLine(Line6,(u8 *)text);	
				sprintf(text,"                  ");
	LCD_DisplayStringLine(Line7,(u8 *)text);
				sprintf(text,"                  ");
	LCD_DisplayStringLine(Line8,(u8 *)text);
//				sprintf(text,"                  ");
//	LCD_DisplayStringLine(Line9,(u8 *)text);
		
				sprintf(text,"   %d            ",mode_canshu);
	LCD_DisplayStringLine(Line9,(u8 *)text);
	}	
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
			if(++key_time==100&&long_state==1)
			{
				
				long_state=0;
			}
		}
		if(last_state==i&&now_state==0)
		{
			if(long_state)key_short=i;
			else
			{
			long_state=1;key_long=i;
			}				
		}
	}
}




