#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_dac.h"
#include "pg12864.h"
#include "led.h"
#include "delay.h"
#include <stdio.h> //Ϊ��ʹ��sprintf����
   

 /**
 *  1��
 *  2��ͬʱ���ڿɽ��յ��Դ������ַ�����һ��Ӣ���ַ�����ʾ��mini12864�ϡ�
 */
 

int main(void)
{  	

	 u8 t;
	 u8 buffer[17];
	
	// ���ȼ�����
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	/* ��ʼ��USART */
	 USART_Config(); 
	
	 delay_init();
	 LED_Init();
	
	 GPIO_Init_12864();
	 Init_12864();
	 ClearScreen();
	
 
	/*��ʼ��DAC����ʼDACת��*/
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
 
