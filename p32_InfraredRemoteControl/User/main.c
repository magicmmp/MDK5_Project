#include "stm32f10x.h"
#include "bsp_usart.h"
#include "pg12864.h"
#include "led.h"
#include "delay.h"
#include "remote.h"
#include <stdio.h> //Ϊ��ʹ��sprintf����


 
 
 /**
 *  1��
 *  2��ͬʱ���ڿɽ��յ��Դ������ַ�����һ��Ӣ���ַ�����ʾ��mini12864�ϡ�
 */
 

int main(void)
{  	

	 u8 t;
	 u8 key; 
	 u8 buffer[17];
	
	/* ��ʼ��USART */
	 USART_Config(); 
	
	 delay_init();
	 LED_Init();

	 Remote_Init();	//������ճ�ʼ��
	
	 GPIO_Init_12864();
	 Init_12864();
	 ClearScreen();
	
	 
	
	
	
	WriteEn(0,0,(u8*)"Key Value:");		//��ʾ��ֵ
	WriteEn(0,2,(u8*)"Long Press:");	//��ʾ��������	
	
	 while(1)
	{
		
		
		Remote_Debug();  //��ӡ������Ϣ������
		key=  Remote_Scan();		
		if(key>0)
		{
			sprintf((char*)buffer, "Key Value: 0x%02X", key);
			WriteEn(0,0,buffer);	//��ʾ��ֵ
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
 
