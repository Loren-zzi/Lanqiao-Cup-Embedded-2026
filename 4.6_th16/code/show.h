#ifndef __show_H
#define __show_H

#include "headfile.h"
//=============================

extern int a,b;
extern volatile uint8_t key_short,key_long;
extern uint8_t mode_lcd;// 0 1 2
extern uint8_t mode_canshu;//0123

extern uint8_t time_star_flag;//0重启
extern uint16_t DS;
extern uint16_t DR, FS,FR;

extern uint16_t test_DS,test_DR,test_FS,test_FR;
extern uint8_t lock_flag;//0解锁   1锁定




float get_vol(ADC_HandleTypeDef *hadc);
void show_led(uint8_t led,uint8_t state);
void key_loop(void);
void task(void);
void show_lcd(void);

#endif
