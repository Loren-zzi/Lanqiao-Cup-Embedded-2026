#include "led_lcd.h"

//====================lcd
void show_led(uint8_t led,uint8_t state);
char text[20];
  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
  uint8_t led8state;
void show_lcd(void)
{
	  HAL_RTC_GetTime(&hrtc,&sTime,RTC_FORMAT_BIN);
	  HAL_RTC_GetDate(&hrtc,&sDate,RTC_FORMAT_BIN);
	sprintf(text,"  Hello,World!");
	LCD_DisplayStringLine(Line0,(uint8_t *)text);
	sprintf(text,"  %2d:%2d:%2d    ",sTime.Hours,sTime.Minutes,sTime.Seconds);
	LCD_DisplayStringLine(Line1,(uint8_t *)text);
	sprintf(text,"  %d-%d-%d      ",sDate.Year,sDate.Month,sDate.WeekDay);
	LCD_DisplayStringLine(Line2,(uint8_t *)text);
	
	show_led(8,led8state);
}
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	led8state=1;
}

//=====================led
void show_led(uint8_t led,uint8_t state)
{
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
	if(state)HAL_GPIO_WritePin(GPIOC,0x0080<<led,GPIO_PIN_RESET);
	else HAL_GPIO_WritePin(GPIOC,0x0080<<led,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
}






