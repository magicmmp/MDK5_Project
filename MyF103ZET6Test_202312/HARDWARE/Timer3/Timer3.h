#ifndef __Timer3_H
#define __Timer3_H	
#include "sys.h"

extern uint8_t flag_T3_Update; //T3中断标志，周期1ms
void TIM3_Int_Init();

 
#endif 