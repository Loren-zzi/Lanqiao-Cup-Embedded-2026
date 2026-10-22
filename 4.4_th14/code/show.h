#ifndef _show_h
#define _show_h


#include "headfile.h"

extern uint8_t mode_lcd;//DATA,
extern uint8_t allow_change_flag;//5s
extern uint8_t mode_fre;//0为低频模式，1为高平模式
extern uint8_t change_RK_flag; //0为R 1为K；;

extern uint8_t R;
extern uint8_t K;

extern uint8_t change_R;
extern uint8_t change_K;

extern uint16_t pwm_count;

extern uint8_t adc_flag;//0 解锁 1 锁定

//================================

void show_led(uint8_t led,uint8_t state);
void show_lcd(void);
void key_loop(void);
float get_vol(ADC_HandleTypeDef *hadc);
void task(void);


extern volatile uint8_t key_short,key_long;


#endif
