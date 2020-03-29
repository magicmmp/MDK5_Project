#include "stm32f10x.h"
#include "bsp_usart.h"
#include "led.h"
#include "can.h"
#include "delay.h"
#include <stdio.h> //Ϊ��ʹ��sprintf����


 
 
 /**
 *  1��CAN�շ�����
 *  2���Ѵ�����ԭ�Ӿ�Ӣ����յ��ֽ����ݼ�10�ٷ���ȥ
 */
 //�Ѳ��Գɹ� 2020-3-29

int main(void)
{  	

	 u8 i,t;
	 u8 nByte;
	
	
	 u8 canbuf[8];
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	/* ��ʼ��USART */
	 USART_Config(); 
	
	 delay_init();
	 LED_Init();

	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_Normal);//CAN��ʼ������ģʽ,������500Kbps 
	 
	
	
	while(1)
	{
		
		
		
		
		nByte=Can_Receive_Msg(canbuf);
		if(nByte)//���յ�������
		{	
			if(nByte>8)
				nByte=8;
			for(i=0;i<nByte;i++)
			{
				canbuf[i]=canbuf[i]+10;//��䷢�ͻ�����
 			}
			Can_Send_Msg(canbuf,nByte);//����8���ֽ�
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
 
