#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_dac.h"
#include "pg12864.h"
#include "led.h"
#include "delay.h"
#include <stdio.h> //为了使用sprintf函数
   

 /**
 *  1、
 *  2、同时串口可接收电脑串口助手发来的一串英文字符，显示在mini12864上。
 */
 

int main(void)
{  	

	 u8 t;
	 u8 buffer[17];
	
	// 优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	/* 初始化USART */
	 USART_Config(); 
	
	 delay_init();
	 LED_Init();
	
	 GPIO_Init_12864();
	 Init_12864();
	 ClearScreen();
	
 
	/*初始化DAC，开始DAC转换*/
	 DAC_Mode_Init();
	
	
	 while(1)
	{
		
		delay_ms(1000);
		t++;
		if(t%2)
		{
			LED0=!LED0;
			LED1=!LED1;
		}
	}


	
}
 
