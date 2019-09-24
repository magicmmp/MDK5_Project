#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_adc.h"
#include "pg12864.h"
#include "led.h"
#include "delay.h"
#include <stdio.h> //Ϊ��ʹ��sprintf����


 extern __IO uint16_t ADC_ConvertedValue;
 
// �ֲ����������ڱ���ת�������ĵ�ѹֵ 	 
float ADC_ConvertedValueLocal;        


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
	 ADCx_Init();
	
	 delay_init();
	 LED_Init();
	
	 GPIO_Init_12864();
	 Init_12864();
	 ClearScreen();
	
	 
	
	
	
	
	printf("\r\n ----����һ��ADC��ͨ���ж϶�ȡʵ��----\r\n");
	
	
	 while(1)
	{
		
		//ADC_ConvertedValueֵ��úܿ�
		ADC_ConvertedValueLocal =(float) ADC_ConvertedValue/4096*3.3; 
	
		sprintf((char*)buffer,     "ADC  test:");
	    WriteEn(0,0,buffer);	//��ʾ��ֵ
		sprintf((char*)buffer,     "Value : 0x%04X", ADC_ConvertedValue);
	    WriteEn(0,2,buffer);	//��ʾ��ֵ
		sprintf((char*)buffer,     "Result: %.3f V", ADC_ConvertedValueLocal);
	    WriteEn(0,4,buffer);	//��ʾ��ֵ
		
		printf("\r\n The current AD value = 0x%04X \r\n", 
		       ADC_ConvertedValue); 
		printf("\r\n The current AD value = %f V \r\n",
		       ADC_ConvertedValueLocal); 
		printf("\r\n\r\n");
		
		delay_ms(1000);
		delay_ms(1000);
		t++;
		if(t%2)
		{
			LED0=!LED0;
			LED1=!LED1;
		}
	}


	
}
 
