#include "stm32f10x.h"
#include "bsp_usart.h"
#include "led.h"
#include "can.h"
#include "delay.h"
#include <stdio.h> //为了使用sprintf函数


 
 
 /**
 *  1、CAN收发测试
 *  2、把从正点原子精英板接收的字节数据加10再发回去
 */
 //已测试成功 2020-3-29

int main(void)
{  	

	 u8 i,t;
	 u8 nByte;
	
	
	 u8 canbuf[8];
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	/* 初始化USART */
	 USART_Config(); 
	
	 delay_init();
	 LED_Init();

	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_Normal);//CAN初始化环回模式,波特率500Kbps 
	 
	
	
	while(1)
	{
		
		
		
		
		nByte=Can_Receive_Msg(canbuf);
		if(nByte)//接收到有数据
		{	
			if(nByte>8)
				nByte=8;
			for(i=0;i<nByte;i++)
			{
				canbuf[i]=canbuf[i]+10;//填充发送缓冲区
 			}
			Can_Send_Msg(canbuf,nByte);//发送8个字节
		}
		else
			delay_ms(10);	
		
		
		
		
		t++;
		if(t%50==0)
		{
			LED0=!LED0;
			LED1=!LED0;
			LED2=!LED1;
		}
	}


	
}
 
