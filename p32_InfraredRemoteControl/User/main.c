#include "stm32f10x.h"
#include "bsp_usart.h"
#include "pg12864.h"
#include "led.h"
#include "delay.h"
#include "remote.h"
#include <stdio.h> //为了使用sprintf函数


 
 
 /**
 *  1、
 *  2、同时串口可接收电脑串口助手发来的一串英文字符，显示在mini12864上。
 */
 

int main(void)
{  	

	 u8 t;
	 u8 key; 
	 u8 buffer[17];
	
	/* 初始化USART */
	 USART_Config(); 
	
	 delay_init();
	 LED_Init();

	 Remote_Init();	//红外接收初始化
	
	 GPIO_Init_12864();
	 Init_12864();
	 ClearScreen();
	
	 
	
	
	
	WriteEn(0,0,(u8*)"Key Value:");		//显示键值
	WriteEn(0,2,(u8*)"Long Press:");	//显示按键次数	
	
	 while(1)
	{
		
		
		Remote_Debug();  //打印脉冲信息到串口
		key=  Remote_Scan();		
		if(key>0)
		{
			sprintf((char*)buffer, "Key Value: 0x%02X", key);
			WriteEn(0,0,buffer);	//显示键值
		}
		else
			delay_ms(10);
		
		
		t++;
		if(t%100==0)
		{
			LED0=!LED0;
			LED1=!LED1;
		}
	}


	
}
 
