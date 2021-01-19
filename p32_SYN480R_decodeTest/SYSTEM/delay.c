#include "delay.h"

uint16_t       delay_time_count_tmp;


//本函数需要配合SYSTick中断使用，先配置SYSTick的中断间隔为1ms
void delay_ms(u16 nms)
{
		delay_time_count_tmp=nms;
		while(delay_time_count_tmp);
} 









































