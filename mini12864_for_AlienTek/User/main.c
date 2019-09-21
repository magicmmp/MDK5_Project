#include "usart.h"
#include "key.h"
#include "delay.h"
#include "sys.h"
#include "pg12864.h"


int main()
{	
	

	delay_init();
	

	GPIO_Init_12864();
	Init_12864();
	ClearScreen();
	while(1)
	{
		WriteEn(0,0,(uint8_t*)"12864 Test");
		WriteEn(0,2,(uint8_t*)"For   AlienTek  Board");
		WriteCn(0,6,(uint8_t*)"确定空空取消");
	}
	
	
}



