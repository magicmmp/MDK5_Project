	
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
			 	
//要写入到24c02的字符串数组
const u8 TEXT_Buffer[]={"2022-11-14 AT24C02 读写测试 Elite STM32 IIC TEST"};
#define SIZE sizeof(TEXT_Buffer)	
	

//2022-1-21  run successfully!
int main()
{		

	
	u8 lcd_id[128];			//存放LCD ID字符串
	u8 i;
	u8 dataBuff[256];
	u8 datatemp[SIZE];
	u8 nBytes;
	u8 isDebug=0;
	u8 key;
	
	//SCB->VTOR = FLASH_BASE | 0x10000;
	
	
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	
	 if(SysTick_Config(SystemCoreClock/1000))	 	/* 1ms */
	 { 
		/* Capture error */ 
		 while (1);
	 }
	 
	 /* 初始化USART */
	 USART_Config(); 
	
	 LED_Init();
	 KEY_Init();
	 AT24CXX_Init();			//IIC初始化
	 LCD_Init();
	 POINT_COLOR=RED;
	 LCD_Clear(BLACK);
	

	 
	 while(AT24CXX_Check())//检测不到24c02
	{
		LCD_ShowString(30,150,200,16,16,"24C02 Check Failed!");
		delay_ms(500);
		LCD_ShowString(30,150,200,16,16,"Please Check!      ");
		delay_ms(500);
		LED0=!LED0;//DS0闪烁
	}
	LCD_ShowString(30,150,200,16,16,"24C02 Ready!");    
 	POINT_COLOR=BLUE;//设置字体为蓝色	  
	
	
	
	
	 
	
	  while (1)
	  {
			
			key=KEY_Scan(0);
			if(key==KEY1_PRES)//KEY_UP按下,写入24C02
			{
				LCD_Fill(0,170,239,319,BLACK);//清除半屏    
				//LCD_ShowString(30,170,200,16,16,"Start Write 24C02....");
				LCD_ShowAsciiString(30,170,(u8*)"Start Write 24C02....",2,GREEN,BLACK);
				AT24CXX_Write(100,(u8*)TEXT_Buffer,SIZE);
				//LCD_ShowString(30,170,200,16,16,"24C02 Write Finished!");//提示传送完成
				LCD_ShowAsciiString(30,170,(u8*)"24C02 Write Finished!",2,GREEN,BLACK);
			}
			if(key==KEY0_PRES)//KEY1按下,读取字符串并显示
			{
				//LCD_ShowString(30,170,200,16,16,"Start Read 24C02.... ");
				LCD_ShowAsciiString(30,170,(u8*)"Start Read 24C02.... ",2,GREEN,BLACK);
				AT24CXX_Read(100,datatemp,SIZE);
				//LCD_ShowString(30,170,200,16,16,"The Data Readed Is:  ");//提示传送完成
				LCD_ShowAsciiString(30,170,(u8*)"The Data Readed Is:  ",2,GREEN,BLACK);
				//LCD_ShowString(30,190,200,16,16,datatemp);//显示读到的字符串
				LCD_ShowAsciiString(30,210,datatemp,2,GREEN,BLACK);
        
        sprintf((char*)lcd_id,"nDataTimes=%-10d",2022);
			}
          
			LED_RUN();
				
			
			LCD_ShowAsciiString(0,0,(u8*)"IIC_24C02_test",2,RED,GRAY);
		//	sprintf((char*)lcd_id,"nDataTimes=%-10d         ",nDataTimes);
				
	
		  
	  
	  }
}

/***********************************END OF FILE*********************************/

