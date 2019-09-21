#include "stm32f10x.h"
#include "bsp_usart_dma.h"
#include "pg12864.h"
#include "led.h"
#include "delay.h"

 
extern uint8_t SendBuff[SENDBUFF_SIZE];//׼����DMA�ķ�ʽ���͵�����
 
 /**
 *  1���������ݴ�SRAMͨ�����ڷ��͵����ԣ�����DMA�ķ�ʽ
 *  2��ͬʱ���ڿɽ��յ��Դ������ַ�����һ��Ӣ���ַ�����ʾ��mini12864�ϡ�
 */
 
int main(void)
{  	
	 uint16_t i;
	
	 LED_Init();
	
	 GPIO_Init_12864();
	 Init_12864();
	 ClearScreen();
	
	delay_init();
	/* ��ʼ��USART */
	 USART_Config(); 

	/* ����ʹ��DMAģʽ */
	 USARTx_DMA_Config();
	 /*��佫Ҫ���͵�����*/
	for(i=0;i<SENDBUFF_SIZE;i++)
	{
		SendBuff[i]	 = 'A'+i%52;
	}

	/*Ϊ��ʾDMA�������ж�CPU���ܴ����������飬����ʹ��DMA�������ݣ����ǳ���
	*��ʱ�����п��ܻᵼ�µ��Զ˴��ڵ������ֻῨ��������ҷɵ������
	*���DMA�����е�ѭ��ģʽ��Ϊ����ģʽ*/		

	/* USART1 �� DMA����TX���� */
	USART_DMACmd(DEBUG_USARTx, USART_DMAReq_Tx, ENABLE);

	/* ��ʱCPU�ǿ��еģ����Ը����������� */  
	//����ͬʱ����LED
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
 
