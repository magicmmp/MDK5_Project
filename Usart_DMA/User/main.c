#include "stm32f10x.h"
#include "bsp_usart_dma.h"
#include "pg12864.h"
#include "led.h"
#include "delay.h"

 
extern uint8_t SendBuff[SENDBUFF_SIZE];//准备用DMA的方式发送的数据
 
 /**
 *  1、测试数据从SRAM通过串口发送到电脑，采样DMA的方式
 *  2、同时串口可接收电脑串口助手发来的一串英文字符，显示在mini12864上。
 */
 
int main(void)
{  	
	 uint16_t i;
	
	 LED_Init();
	
	 GPIO_Init_12864();
	 Init_12864();
	 ClearScreen();
	
	delay_init();
	/* 初始化USART */
	 USART_Config(); 

	/* 配置使用DMA模式 */
	 USARTx_DMA_Config();
	 /*填充将要发送的数据*/
	for(i=0;i<SENDBUFF_SIZE;i++)
	{
		SendBuff[i]	 = 'A'+i%52;
	}

	/*为演示DMA持续运行而CPU还能处理其它事情，持续使用DMA发送数据，量非常大，
	*长时间运行可能会导致电脑端串口调试助手会卡死，鼠标乱飞的情况，
	*或把DMA配置中的循环模式改为单次模式*/		

	/* USART1 向 DMA发出TX请求 */
	USART_DMACmd(DEBUG_USARTx, USART_DMAReq_Tx, ENABLE);

	/* 此时CPU是空闲的，可以干其他的事情 */  
	//例如同时控制LED
	while(1)
	{
		if(USART_RX_STA&0x8000)
		{	
			ClearScreen();			
			WriteEn(0,0,USART_RX_BUF);
			USART_RX_STA=0;
			LED0=!LED0;
			LED1=!LED1;
		}
		
	}
}
 
