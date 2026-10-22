#ifndef __show_H
#define __show_H


#include "headfile.h"
//========================


extern volatile uint8_t key_short,key_long;

void show_led(uint8_t led,uint8_t state);
void key_loop(void);
void task(void);
void show_lcd(void);
float get_vol(ADC_HandleTypeDef *hadc);

extern uint8_t mode_lcd;//0 1
extern uint8_t auto_flag;//1为自动
extern uint16_t v;//转速
extern float temp,temp_set;//温度
 void uart_process(void);
 extern uint8_t over_flag;//0超速

#endif
