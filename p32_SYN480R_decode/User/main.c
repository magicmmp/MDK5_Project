	
#include "stm32f10x.h"

#include "led.h"
#include "sys.h"
#include "lcd.h"
#include "stdlib.h"
#include "font.h" 
#include "bsp_usart.h"
#include "syn480_decode.h"
#include "key.h"

//用于打印字节到显示屏
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
	

	 
	 
	
	
	
	
	 
	
	  while (1)
	  {
			
			key=KEY_Scan(0);
			if(key==KEY0_PRES)
			{
				isDebug++;
				isDebug=isDebug%3;
			}
				
			if(isDebug==0)
			{
				LCD_ShowAsciiString(0,0,(u8*)"Press  Key0 to show    data",2,RED,GRAY);
				
				
				error=syn480_getErrNo();
				if(error)
				{
					nDataTimes++;
					if(error==1)
					{
					//	printf("数据有误，有干扰");
						LCD_ShowAsciiString(0,160,(u8*)"             OK               ",2,RED,GREEN);
					}
					else if(error==2)
					{
						nDataError++;
						LCD_ShowAsciiString(0,160,(u8*)"Error, data transmission error",2,YELLOW,RED);
					}
					else if(error==3)
					{
						nDataError++;
						LCD_ShowAsciiString(0,160,(u8*)"Error, bit time overflow      ",2,YELLOW,RED);
					}
				}
				
				nBytes=syn480_GetData(dataBuff);
				if(nBytes>0)
				{
					preData++;
					if(preData!=dataBuff[0])
					{
						//printf("数据有误，上一帧数据丢失");
						//printf("\r\n");
						LCD_ShowAsciiString(0,160,(u8*)"Error, last data frame missing",2,YELLOW,RED);
						nDataError++;
					}
					preData=dataBuff[0];
					
					//printf("收到 %d 字节数据：",nBytes);
					//printf("\r\n");
					
					sprintf((char*)lcd_id,"get data bytes = %3d          ",nBytes);
					LCD_ShowAsciiString(0,196,lcd_id,2,RED,BLUE);
				
					for(i=0;i<24;i++)
					{
						printDataToArray(lcd_id,dataBuff[i],i*5);
					}
					for(i=nBytes*5;i<128;i++)
					{
						lcd_id[i]=' ';
					}		
					lcd_id[120]=0;					
					LCD_ShowAsciiString(0,228,lcd_id,2,RED,YELLOW);
					
					
				}
			}
			else if(isDebug==1)
			{
				error=syn480_getErrNo();
				if(error)
				{
					nDataTimes++;
					
					if(error==1)
					{
						printf("数据正确");
					}
					else if(error==2)
					{
						nDataError++;
						printf("数据有误，有干扰");
					}
					else if(error==3)
					{
						nDataError++;
						printf("数据有误，位时间持续过长");
					}
					printf("\r\n");
				}
				
				nBytes=syn480_GetData(dataBuff);
				if(nBytes>0)
				{
					preData++;
					if(preData!=dataBuff[0])
					{
						printf("数据有误，上一帧数据丢失");
						printf("\r\n");
						nDataError++;
					}
					preData=dataBuff[0];
					
					printf("收到 %d 字节数据：",nBytes);
					printf("\r\n");
						
						for(i=0;i<nBytes;i++)
							printf("% 3d",dataBuff[i]);
						
						printf("\r\n\r\n");
				}
				LCD_ShowAsciiString(0,0,(u8*)"Press Key0 to show    Debug",2,RED,GRAY);
			}
			else if(isDebug==2)
			{
				LCD_ShowAsciiString(0,0,(u8*)"Press Key0 to turn off UART",2,RED,GRAY);
				Remote_Debug();
			}
			
			
			
			
          
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

