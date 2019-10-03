#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_adc.h"
#include "pg12864.h"
#include "led.h"
#include "delay.h"
#include <stdio.h> //Ϊ��ʹ��sprintf����

// ADC1ת���ĵ�ѹֵͨ��MDA��ʽ����SRAM
extern __IO uint16_t ADC_ConvertedValue[NOFCHANEL];

// �ֲ����������ڱ���ת�������ĵ�ѹֵ 	 
float ADC_ConvertedValueLocal[NOFCHANEL];      

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
	
	 
	
	
	// ADC ��ʼ��
	ADCx_Init();
	
	printf("\r\n ----����һ��ADC��ͨ���ɼ�ʵ��----\r\n");
	
	
	 while(1)
	{
		ADC_ConvertedValueLocal[0] =(float) ADC_ConvertedValue[0]/4096*3.3;
		ADC_ConvertedValueLocal[1] =(float) ADC_ConvertedValue[1]/4096*3.3;
		ADC_ConvertedValueLocal[2] =(float) ADC_ConvertedValue[2]/4096*3.3;
		ADC_ConvertedValueLocal[3] =(float) ADC_ConvertedValue[3]/4096*3.3;
		ADC_ConvertedValueLocal[4] =(float) ADC_ConvertedValue[4]/4096*3.3;
		ADC_ConvertedValueLocal[5] =(float) ADC_ConvertedValue[5]/4096*3.3;
		ADC_ConvertedValueLocal[6] =(float) ADC_ConvertedValue[6]/4096*3.3;
		ADC_ConvertedValueLocal[7] =(float) ADC_ConvertedValue[7]/4096*3.3;
	
		printf("\r\n CH0 value = %f V \r\n",ADC_ConvertedValueLocal[0]);
		printf("\r\n CH1 value = %f V \r\n",ADC_ConvertedValueLocal[1]);
		printf("\r\n CH2 value = %f V \r\n",ADC_ConvertedValueLocal[2]);
		printf("\r\n CH3 value = %f V \r\n",ADC_ConvertedValueLocal[3]);
		printf("\r\n CH4 value = %f V \r\n",ADC_ConvertedValueLocal[4]);
		printf("\r\n CH5 value = %f V \r\n",ADC_ConvertedValueLocal[5]);
		printf("\r\n CH6 value = %f V \r\n",ADC_ConvertedValueLocal[6]);
		printf("\r\n CH7 value = %f V \r\n",ADC_ConvertedValueLocal[7]);
		printf("\r\n\r\n");
	
		
		for(t=0;t<8;t++)
		{
			sprintf((char*)buffer,     "A%d %.2f",t,ADC_ConvertedValueLocal[t]);
			buffer[7]=0;
			WriteEn(64*(t&0x1),(t/2)*2,buffer);	//��ѹֵ
		}
		

		
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
 
