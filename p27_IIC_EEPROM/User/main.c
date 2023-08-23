	
#include "stm32f10x.h"

#include "led.h"
#include "sys.h"
#include "lcd.h"
#include "stdlib.h"
#include "font.h" 
#include "bsp_usart.h"
#include "key.h"
#include "24cxx.h"
#include "delay.h"
			 	
//Ҫд�뵽24c02���ַ�������
const u8 TEXT_Buffer[]={"2022-11-14 AT24C02 ��д���� Elite STM32 IIC TEST"};
#define SIZE sizeof(TEXT_Buffer)	
	

//2022-1-21  run successfully!
int main()
{		

	
	u8 lcd_id[128];			//���LCD ID�ַ���
	u8 i;
	u8 dataBuff[256];
	u8 datatemp[SIZE];
	u8 nBytes;
	u8 isDebug=0;
	u8 key;
	
	//SCB->VTOR = FLASH_BASE | 0x10000;
	
	
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	
	 if(SysTick_Config(SystemCoreClock/1000))	 	/* 1ms */
	 { 
		/* Capture error */ 
		 while (1);
	 }
	 
	 /* ��ʼ��USART */
	 USART_Config(); 
	
	 LED_Init();
	 KEY_Init();
	 AT24CXX_Init();			//IIC��ʼ��
	 LCD_Init();
	 POINT_COLOR=RED;
	 LCD_Clear(BLACK);
	

	 
	 while(AT24CXX_Check())//��ⲻ��24c02
	{
		LCD_ShowString(30,150,200,16,16,"24C02 Check Failed!");
		delay_ms(500);
		LCD_ShowString(30,150,200,16,16,"Please Check!      ");
		delay_ms(500);
		LED0=!LED0;//DS0��˸
	}
	LCD_ShowString(30,150,200,16,16,"24C02 Ready!");    
 	POINT_COLOR=BLUE;//��������Ϊ��ɫ	  
	
	
	
	
	 
	
	  while (1)
	  {
			
			key=KEY_Scan(0);
			if(key==KEY1_PRES)//KEY_UP����,д��24C02
			{
				LCD_Fill(0,170,239,319,BLACK);//�������    
				//LCD_ShowString(30,170,200,16,16,"Start Write 24C02....");
				LCD_ShowAsciiString(30,170,(u8*)"Start Write 24C02....",2,GREEN,BLACK);
				AT24CXX_Write(100,(u8*)TEXT_Buffer,SIZE);
				//LCD_ShowString(30,170,200,16,16,"24C02 Write Finished!");//��ʾ�������
				LCD_ShowAsciiString(30,170,(u8*)"24C02 Write Finished!",2,GREEN,BLACK);
			}
			if(key==KEY0_PRES)//KEY1����,��ȡ�ַ�������ʾ
			{
				//LCD_ShowString(30,170,200,16,16,"Start Read 24C02.... ");
				LCD_ShowAsciiString(30,170,(u8*)"Start Read 24C02.... ",2,GREEN,BLACK);
				AT24CXX_Read(100,datatemp,SIZE);
				//LCD_ShowString(30,170,200,16,16,"The Data Readed Is:  ");//��ʾ�������
				LCD_ShowAsciiString(30,170,(u8*)"The Data Readed Is:  ",2,GREEN,BLACK);
				//LCD_ShowString(30,190,200,16,16,datatemp);//��ʾ�������ַ���
				LCD_ShowAsciiString(30,210,datatemp,2,GREEN,BLACK);
        
        sprintf((char*)lcd_id,"nDataTimes=%-10d",2022);
			}
          
			LED_RUN();
				
			
			LCD_ShowAsciiString(0,0,(u8*)"IIC_24C02_test",2,RED,GRAY);
		//	sprintf((char*)lcd_id,"nDataTimes=%-10d         ",nDataTimes);
				
	
		  
	  
	  }
}

/***********************************END OF FILE*********************************/

