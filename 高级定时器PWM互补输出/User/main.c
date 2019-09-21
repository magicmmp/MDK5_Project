#include "stm32f10x.h"
#include "bsp_usart.h"
#include "pg12864.h"
#include "delay.h"
#include "bsp_AdvanceTim.h" 



 
 
 
 /**
 *  1��
 *  2��ͬʱ���ڿɽ��յ��Դ������ַ�����һ��Ӣ���ַ�����ʾ��mini12864�ϡ�
 */
 

int main(void)
{  	
		 	
	delay_init();

	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	
	
	 GPIO_Init_12864();
	 Init_12864();
	 ClearScreen();
	
	 
	
	
	/* ��ʼ��USART */
	 USART_Config(); 
	
	/* �߼���ʱ����ʼ�� */
	
	ADVANCE_TIM_Init();
	while(1)
	{  
		
	}


	
}
 
