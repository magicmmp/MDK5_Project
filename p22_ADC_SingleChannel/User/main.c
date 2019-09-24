#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_adc.h"
#include "pg12864.h"
#include "led.h"
#include "delay.h"
#include <stdio.h> //为了使用sprintf函数


 extern __IO uint16_t ADC_ConvertedValue;
 
// 局部变量，用于保存转换计算后的电压值 	 
float ADC_ConvertedValueLocal;        


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
	 ADCx_Init();
	
	 delay_init();
	 LED_Init();
	
	 GPIO_Init_12864();
	 Init_12864();
	 ClearScreen();
	
	 
	
	
	
	
	printf("\r\n ----这是一个ADC单通道中断读取实验----\r\n");
	
	
	 while(1)
	{
		
		//ADC_ConvertedValue值变得很快
		ADC_ConvertedValueLocal =(float) ADC_ConvertedValue/4096*3.3; 
	
		sprintf((char*)buffer,     "ADC  test:");
	    WriteEn(0,0,buffer);	//显示键值
		sprintf((char*)buffer,     "Value : 0x%04X", ADC_ConvertedValue);
	    WriteEn(0,2,buffer);	//显示键值
		sprintf((char*)buffer,     "Result: %.3f V", ADC_ConvertedValueLocal);
	    WriteEn(0,4,buffer);	//显示键值
		
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
 
