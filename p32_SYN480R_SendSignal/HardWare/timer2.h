#ifndef __TIMER2_H
#define __TIMER2_H
#include "sys.h"

void TIM2_PWM_Init();
void NECSendByte();

extern uint16_t test_time2;
extern u8 bitsToSend;

#endif