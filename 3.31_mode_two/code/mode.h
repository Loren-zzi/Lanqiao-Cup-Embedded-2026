#ifndef _mode_h
#define _mode_h

#include "headfile.h"


void show_led(uint8_t led,uint8_t state);
void show_lcd(void);

extern uint8_t led8state;
extern int16_t highlignt;
extern int16_t count;
extern volatile uint8_t key_short,key_long;

extern uint8_t rec_data;
void uart_rec(void);

#endif

