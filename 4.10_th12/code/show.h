#ifndef __show_H
#define __show_H

#include "headfile.h"
//=============================

extern int a,b;
extern volatile uint8_t key_short,key_long;

extern uint8_t mode_lcd;//0  1




extern float m_cnbr,m_vnbr;

void uart_process(void);



void show_led(uint8_t led,uint8_t state);
void key_loop(void);
void task(void);
void show_lcd(void);

#endif
