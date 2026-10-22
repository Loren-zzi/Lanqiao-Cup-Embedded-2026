#ifndef _mode_one_h
#define _mode_one_h

#include "headfile.h"

void show_led(uint8_t led,uint8_t state);

void key_loop(void);
extern volatile uint8_t key_long;
extern volatile uint8_t key_short;

extern uint8_t led8_state;
void show_lcd(void);
extern uint8_t highlight;
extern int  count;

#endif
