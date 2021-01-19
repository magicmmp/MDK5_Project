	
#include "stm32f10x.h"

#include "led.h"
#include "sys.h"
#include "lcd.h"
#include "stdlib.h"
#include "font.h" 
#include "w25qxx.h"

//要写入到W25Q64的字符串数组
const u8 TEXT_Buffer[]={"ELITE STM32 SPI TEST"};
#define SIZE sizeof(TEXT_Buffer)


int main()
{		

	u8 x=0;
	u8 lcd_id[64];			//存放LCD ID字符串
	u32 i;
	u8 datatemp[SIZE];
	u32 FLASH_SIZE; 
	
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	
	 if(SysTick_Config(SystemCoreClock/1000))	 	/* 1ms */
	 { 
		/* Capture error */ 
		 while (1);
	 }
	 LED_Init();
	 W25QXX_Init();			//W25QXX初始化 
	 LCD_Init();

	while(W25QXX_ReadID()!=W25Q128)								//检测不到W25Q128
	{	
		LCD_ShowAsciiString(0,0,(u8*)"W25Q128 Check Failed!",2,BLUE,GRAY);
		LCD_ShowAsciiString(0,128,(u8*)"W25Q128 Check Failed!",2,RED,GRAY);	
	}

  LCD_ShowAsciiString(0,160,(u8*)"W25Q128 Ready!",2,RED,GRAY);	
	FLASH_SIZE=128*1024*1024;	//FLASH 大小为16M字节
 	POINT_COLOR=BLUE;//设置字体为蓝色	  
	
	LCD_ShowAsciiString(0,200,(u8*)"Start Write W25Q128....",2,RED,GRAY);	
	W25QXX_Write((u8*)TEXT_Buffer,FLASH_SIZE-100,SIZE);			//从倒数第100个地址处开始,写入SIZE长度的数据
	LCD_ShowAsciiString(0,250,(u8*)"W25Q128 Write Finished!",2,RED,GRAY);	//提示传送完成
	
	
	LCD_ShowAsciiString(0,300,(u8*)"Start Read W25Q128.... ",2,RED,GRAY);	
	W25QXX_Read(datatemp,FLASH_SIZE-100,SIZE);					//从倒数第100个地址处开始,读出SIZE个字节
	
	LCD_ShowAsciiString(0,350,(u8*)"The Data Readed Is:  ",2,RED,GRAY);	
	
	LCD_ShowAsciiString(0,400,datatemp,2,RED,GRAY);	//显示读到的字符串
	 
	
	  while (1)
	  {
          
			if(led_time_count_tmp==0)
			{
				led_time_count_tmp=led_time_count;
				LED0=!LED0;
				LED1=!LED0;				
				
				if(1)
				{				
					LCD_ShowCnString(0,500,(u8*)"我们日历年月星期设置一二三们四五六时分秒上下左右电子",2,RED,GRAY);
					LCD_ShowAsciiString(0,700,(u8*)"abcDSE%^NNbbbbb",2,RED,GRAY);
				
				}
	
			 }  
	  
	  }
}

/***********************************END OF FILE*********************************/

