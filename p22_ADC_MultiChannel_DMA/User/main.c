#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_adc.h"
#include "pg12864.h"
#include "led.h"
#include "delay.h"
#include <stdio.h> //为了使用sprintf函数

// ADC1转换的电压值通过MDA方式传到SRAM
extern __IO uint16_t ADC_ConvertedValue[NOFCHANEL];

// 局部变量，用于保存转换计算后的电压值 	 
float ADC_ConvertedValueLocal[NOFCHANEL];      

 /**
 *  1、
 *  2、同时串口可接收电脑串口助手发来的一串英文字符，显示在mini12864上。
 */
 

int main(void)
{  	

	 u8 t;
	 u8 buffer[17];
	
	// 优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	/* 初始化USART */
	 USART_Config(); 
	
	 delay_init();
	 LED_Init();
	
	 GPIO_Init_12864();
	 Init_12864();
	 ClearScreen();
	
	 
	
	
	// ADC 初始化
	ADCx_Init();
	
	printf("\r\n ----这是一个ADC多通道采集实验----\r\n");
	
	
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
			WriteEn(64*(t&0x1),(t/2)*2,buffer);	//电压值
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
 
