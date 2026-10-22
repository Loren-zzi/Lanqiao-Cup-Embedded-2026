#ifndef __key_H
#define __key_H


#include "headfile.h"


extern volatile uint8_t key_long;
extern volatile uint8_t key_short;


void key_loop(void);


#endif
