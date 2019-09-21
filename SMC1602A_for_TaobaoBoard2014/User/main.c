#include "usart.h"
#include "key.h"
#include "led.h"
#include "delay.h"
#include "sys.h"
#include "key.h"
#include "smc1602A.h"

//注意只使用了GPIO_D的引脚

int main()
{	
	
	uint8_t line[16];	
	int i;
	uint8_t key;
	int N=0;

	delay_init();
	KEY_Init();         	//初始化与按键连接的硬件接口
	LED_Init();		  		//初始化与LED连接的硬件接口
	GPIO_Init_1602();
	initial_1602();
	for(i=16;i<40;i++)
		writeByteToRAM_1602(ROW_1,i,'A'+i-16);
	for(i=16;i<40;i++)
		writeByteToRAM_1602(ROW_2,i,'a'+i-16);		
	
	while(1)
	{		
		for(i=0;i<16;i++)
			line[i]=i+'!';			
		showOn_1602(ROW_1,line);
		
		
		for(i=0;i<16;i++)
			line[i]=i+'!'+16;	
		
		if(N<40)		
			line[15]=readByteFromRAM_1602(ROW_1,N);
		else
			line[15]=readByteFromRAM_1602(ROW_2,N-40);
			
		showOn_1602(ROW_2,line);
		
		delay_ms(500);		
	
		N=(N+1)%80;
		
		key=KEY_Scan(0);	//得到键值
	   	if(key)
		{						   
			switch(key)
			{				 

				case KEY1_PRES:	//控制LED1翻转	 
					LED0=!LED0;
					break;
				case KEY0_PRES:	//同时控制LED0,LED1翻转 
					LED1=!LED1;
					break;
			}
		}
	}

}



