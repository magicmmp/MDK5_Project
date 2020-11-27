#include "stm32f10x.h"
#include "led.h"
#include "all_debug_var.h"	
#include <stdio.h> //Ϊ��ʹ��sprintf����


 

int main(void)
{  	
	
	int i;
	

	 SystemInit();/* ��λ����ʱ�����ã�����ϵͳʱ��72M */
	
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�жϷ���
	
	 if(SysTick_Config(SystemCoreClock/1000))	 	/* 1ms */
	 { 
		/* Capture error */ 
		 while (1);
	 }
	
	
	 LED_Init();

	 

//Ҫ�ȳ�ʼ���ź�����
	 while(1)
	{	
	
		if(led_time_count_tmp==0)
		{
			led_time_count_tmp=led_time_count;
			LED0=!LED0;
			LED1=!LED0;
			
			
			/**
			g_Union_ModBus_3x_data.uint16[0]++;
			g_Union_ModBus_3x_data.uint16[124]--;//���x��32λ�з�������Ҳ���߼�����
			g_Union_ModBus_4x_data.uint16[0]++;
	    g_Union_ModBus_4x_data.uint16[124]--;
			
			if(g_ModBus_0x_data[0]&0x1)
			{
				g_ModBus_0x_data[0]=0;
			}
			else
			{
				g_ModBus_0x_data[0]=0x1;
			}
			g_ModBus_0x_data[248]++;
			g_ModBus_0x_data[249]++;
			g_ModBus_1x_data[248]++;
			g_ModBus_1x_data[249]++;
			*/

		}
		
		

	}

}
 
