#include "stm32f10x.h"
#include "bsp_usart.h"
#include "pg12864.h"
#include "delay.h"
#include "bsp_AdvanceTim.h" 



 
 
 
 /**
 *  1、
 *  2、同时串口可接收电脑串口助手发来的一串英文字符，显示在mini12864上。
 */
 

int main(void)
{  	
		 	
	delay_init();

	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	
	
	 GPIO_Init_12864();
	 Init_12864();
	 ClearScreen();
	
	 
	
	
	/* 初始化USART */
	 USART_Config(); 
	
	/* 高级定时器初始化 */
	
	ADVANCE_TIM_Init();
	while(1)
	{  
		
	}


	
}
 
