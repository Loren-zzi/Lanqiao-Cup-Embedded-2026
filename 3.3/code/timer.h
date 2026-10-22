#ifndef __timer_h
#define __timer_h

#include "headfile.h"

extern uint8_t led7_state;
extern uint32_t fre,capture_val;
extern uint32_t fre2,capture_val2;

extern uint8_t  rec_date;

void uart_date_rec(void);

#endif
