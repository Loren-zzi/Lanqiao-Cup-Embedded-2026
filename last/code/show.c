#include "show.h"



void task(void)
{
show_led(1,1);
}













//===================================




//=========================================
void show_led(uint8_t led,uint8_t state)
{
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
	if(state)HAL_GPIO_WritePin(GPIOC,0x0080<<led,GPIO_PIN_RESET);
	else HAL_GPIO_WritePin(GPIOC,0x0080<<led,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
}
