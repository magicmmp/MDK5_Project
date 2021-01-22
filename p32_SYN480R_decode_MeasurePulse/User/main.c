	
#include "stm32f10x.h"

#include "led.h"
#include "sys.h"
#include "lcd.h"
#include "stdlib.h"
#include "font.h" 
#include "bsp_usart.h"
#include "syn480_decode.h"
#include "key.h"

void printDataToArray(u8*buff,u8 data,u8 index)
{
	u8 ii;
	u8 tmpBuff[6];
	sprintf((char*)tmpBuff,"  %3d",data);
	for(ii=0;ii<5;ii++)
		buff[index+ii]=tmpBuff[ii];
}



int main()
{		

	
	u8 lcd_id[128];			//存放LCD ID字符串
	u8 i;
	u16 nSysTick2;
	u8 dataBuff[256];
	u8 nBytes;
	u8 isDebug=0;
	u8 key;
	u32 nDataError=0;
	u32 nDataTimes=0;
	u8 preData;
	u8 error;
	
	//SCB->VTOR = FLASH_BASE | 0x10000;
	
	
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	
	 if(SysTick_Config(SystemCoreClock/1000))	 	/* 1ms */
	 { 
		/* Capture error */ 
		 while (1);
	 }
	 
	 /* 初始化USART */
	 USART_Config(); 
	 syn480_Init();
	 LED_Init();
	 KEY_Init();
	LCD_Init();
	POINT_COLOR=RED;
	LCD_Clear(BLACK);
	
//printf("num= %-3d, 高电平= %-6d uS ,低电平= %-6d uS\r\n",1,10,20);
	 
	 
	
	
	
	
	 
	
	  while (1)
	  {
			
			key=KEY_Scan(0);
			if(key==KEY0_PRES)
			{
				isDebug++;
				isDebug=isDebug%3;
			}
			
			Remote_Debug();

			
			
			
			
          
			LED_RUN();
				
			
			sprintf((char*)lcd_id,"nDataError=%-6d,Ratio=%5.1f%%",nDataError,nDataError*100.0/(nDataTimes+0.1));
			LCD_ShowAsciiString(0,32,lcd_id,2,RED,YELLOW);
			sprintf((char*)lcd_id,"nDataTimes=%-10d         ",nDataTimes);
			LCD_ShowAsciiString(0,64,lcd_id,2,RED,YELLOW);
			
			sprintf((char*)lcd_id,"nSysTick=%5d",nSysTick2);
			LCD_ShowAsciiString(0,96,lcd_id,2,RED,YELLOW);
			sprintf((char*)lcd_id,"nTIM2_CCR=%5d",nTIM2_CCR);
			LCD_ShowAsciiString(0,128,lcd_id,2,RED,YELLOW);
			
			
			
			
			
			
			
			
		//	LCD_ShowAsciiString(0,500,(u8*)"abcDSE%^NNbbbbb",2,RED,GRAY);
						
				
	
		  
	  
	  }
}

/***********************************END OF FILE*********************************/

