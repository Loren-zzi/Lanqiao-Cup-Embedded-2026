#ifndef __led_key_h
#define __led_key_h

#include "headfile.h"
//=============================led
void show_led(uint8_t led,uint8_t state);
//==============================lcd
extern uint8_t highlight_line;

void show_lcd(void);
double get_vol(ADC_HandleTypeDef *hadc);



//================================key
extern volatile uint8_t key_short;   
extern volatile uint8_t key_long;

uint8_t key_state(void);
void key_loop(void);
//===============================

#endif

