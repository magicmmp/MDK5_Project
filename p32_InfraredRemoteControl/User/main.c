#include "stm32f10x.h"
#include "bsp_usart.h"
#include "pg12864.h"
#include "led.h"
#include "delay.h"
#include "remote.h"
#include "key.h"
#include <stdio.h> //Ϊ��ʹ��sprintf����


 
 
 /**
 *  1��
 *  2��ͬʱ���ڿɽ��յ��Դ������ַ�����һ��Ӣ���ַ�����ʾ��mini12864�ϡ�
 */
 

int main(void)
{  	

	 u8 t;
	 u8 key;
	 u8 isDebug=0; //ѡ���Ƿ񽫺����������Ϣ��ӡ������
	 u8 buffer[17];
	
	/* ��ʼ��USART */
	 USART_Config(); 
	
	 delay_init();
	 LED_Init();

	 KEY_Init();
	 Remote_Init();	//������ճ�ʼ��
	
	 GPIO_Init_12864();
	 Init_12864();
	 ClearScreen();
	
	 
	
	
	
	WriteEn(0,0,(u8*)"Key Value:");		//��ʾ��ֵ
	WriteEn(0,2,(u8*)"Long Press:");	//��ʾ��������
	
	
	 while(1)
	{
		
		key=KEY_Scan(0); //�˺���������ʱ10mS
		if(key==KEY0_PRES)
		{
			if(isDebug)
				isDebug=0;
			else
				isDebug=1;
		}
			
		isDebug=1;
		if(isDebug)  //��ӡ������Ϣ������	
		{
			WriteEn(0,4,(u8*)"Press Key0 to display here");	//��ʾ��������
			Remote_Debug();  		
		}
		else  //��ֵ��ʾ��12864��
		{
			WriteEn(0,4,(u8*)"Press Key0 print to UART  ");	//��ʾ��������	
			
			key=  IR_Remote_to_key();		
			if(key>0)
			{
				sprintf((char*)buffer,     "Key  Value: 0x%02X", key);
				WriteEn(0,0,buffer);	//��ʾ��ֵ
				while(IR_info.isPressing)
				{
					WriteEn(0,2,buffer);	//��ʾ��ֵ
				}
				
			}
			else
				delay_ms(10);
		}
	
		
		t++;
		if(t%50==0)
		{
			LED0=!LED0;
			LED1=!LED1;
		}
	}


	
}
 
