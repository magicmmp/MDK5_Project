#include "delay.h"

uint16_t       delay_time_count_tmp;


//��������Ҫ���SYSTick�ж�ʹ�ã�������SYSTick���жϼ��Ϊ1ms
void delay_ms(u16 nms)
{
		delay_time_count_tmp=nms;
		while(delay_time_count_tmp);
} 









































