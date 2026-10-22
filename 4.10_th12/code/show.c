#include "show.h"

int a,b;

uint8_t mode_lcd;//0  1
uint8_t cn_exist; //0~10
uint8_t vn_exist;
uint8_t idle;


char id[9][5];
char type[9][5];
float in_time[20],out_time[20];
uint8_t dex;//0~8 9  第九个车位给对比用的
int year,mon,sec;
int day[9],hour[9],min[9];
int use_time;
float use_momey;
uint8_t full_flag;//0空   1满

float m_cnbr=3.50,m_vnbr=2.00;



void task(void)
{
	
	
	if(mode_lcd==0)
	{
		idle=8-cn_exist-vn_exist;
		
	}if(idle==0)show_led(1,0);else show_led(1,1);
	
	
	show_lcd();
	if(!full_flag)show_led(1,1);
	else show_led(1,0);

}
void uart_process(void)
{
	dex=8;
	if(((sscanf((char *)rx_buff,"%[^:]:%[^:]:%2d%2d%2d%2d%2d%2d",type[dex],id[dex],&year,&mon,&day[8],&hour[8],&min[8],&sec)==8))&&strlen(rx_buff)==22)
	{
		//printf("%s",id[dex]);
	}
	else printf("Error\r\n");
	for(uint8_t i=0;i<8;i++)
	{
		if((strcmp(type[i],type[8]))==0&&(strcmp(id[i],id[8]))==0)//已经存在、出车!
		{
			use_time=(((day[8]*24*60+hour[8]*60+min[8]))-((day[i]*24*60+hour[i]*60+min[i]))+59)/60;
			//printf("ok\r\n");
			if(type[8][0]=='C')
			{
			use_momey=use_time*m_cnbr;
				printf("%s:%s:%d:%.2f",type[8],id[8],use_time,use_momey);
				cn_exist--;
				memset(type[i],0,5);
				memset(id[i],0,5);	day[i]=0;	hour[i]=0;	min[i]=0;	
			}
			else if(type[i][0]=='V')
			{
			use_momey=use_time*m_vnbr;
			printf("%s:%s:%d:%.2f",type[8],id[8],use_time,use_momey);
				vn_exist--;				memset(type[i],0,5);
				memset(id[i],0,5);	day[i]=0;	hour[i]=0;	min[i]=0;
			}break;
		}
		if(i==7)//不存在   存车！
		{
			if(type[dex][0]=='C')cn_exist++;
			else if(type[dex][0]=='V')vn_exist++;
			for(uint8_t j=0;j<=8;j++)//找出空缺索引
			{
				if(id[j][0]=='\0')
				{
					dex=j;
					full_flag=0;
					strcpy(type[dex],type[8]);
					strcpy(id[dex],id[8]);	
					day[dex]=day[8];hour[dex]=hour[8];min[dex]=min[8];
					break;
				}
				if(j==7)full_flag=1;
			}
			
		}
		//else printf("no\r\n");
	}
		
	
	
}



//===============================LCD
char text[20];
void show_lcd(void)
{
	if(mode_lcd==0)
	{
	sprintf(text,"       Data      ");
	LCD_DisplayStringLine(Line1,(u8 *)text);
	
		sprintf(text,"   CNBR:%d",cn_exist);
	LCD_DisplayStringLine(Line3,(u8 *)text);	
		sprintf(text,"   VNBR:%d",vn_exist);
	LCD_DisplayStringLine(Line5,(u8 *)text);	
		sprintf(text,"   IDLE:%d",idle);
	LCD_DisplayStringLine(Line7,(u8 *)text);		
	}

	if(mode_lcd)
	{
	sprintf(text,"       Para      ");
	LCD_DisplayStringLine(Line1,(u8 *)text);
	
		sprintf(text,"   CNBR:%.2f",m_cnbr);
	LCD_DisplayStringLine(Line3,(u8 *)text);	
		sprintf(text,"   VNBR:%.2f",m_vnbr);
	LCD_DisplayStringLine(Line5,(u8 *)text);	
		sprintf(text,"              ");
	LCD_DisplayStringLine(Line7,(u8 *)text);		
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




