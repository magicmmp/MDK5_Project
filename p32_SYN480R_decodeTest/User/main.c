	
#include "stm32f10x.h"

#include "led.h"
#include "sys.h"
#include "lcd.h"
#include "stdlib.h"
#include "font.h" 
#include "bsp_usart.h"
#include "syn480_decode.h"
#include "key.h"

int main()
{		

	
	u8 lcd_id[64];			//���LCD ID�ַ���
	u8 i;
	u16 nSysTick2;
	u8 dataBuff[256];
	u8 nBytes;
	u8 isDebug=0;
	u8 key;
	u32 nDataError=0;
	u32 nDataTimes=0;
	
	//SCB->VTOR = FLASH_BASE | 0x10000;
	
	
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	
	 if(SysTick_Config(SystemCoreClock/1000))	 	/* 1ms */
	 { 
		/* Capture error */ 
		 while (1);
	 }
	 
	 /* ��ʼ��USART */
	 USART_Config(); 
	 syn480_Init();
	 LED_Init();
	 KEY_Init();
	LCD_Init();
	POINT_COLOR=RED;
	LCD_Clear(BLACK);
	

	 
	 
	
	
	
	
	 
	
	  while (1)
	  {
			
			if(PB10)
			{
				nSysTick=0;
			}
			if(nSysTick>12)
			{
				nSysTick2=nSysTick;
				nSysTick=0;
				isON=1;
				// ʹ�ܼ�����
				//TIM_ITConfig (TIM2, TIM_IT_Update | TIM_IT_CC3, ENABLE );
			}
			
			key=KEY_Scan(0);
			if(key==KEY0_PRES)
			{
				if(isDebug==0)
					isDebug=1;
				else
					isDebug=0;
			}
				
			
			if(isDebug)
			{
				LCD_ShowAsciiString(0,0,(u8*)"Press Key0 to show  data",2,RED,GRAY);
				Remote_Debug();
			}
			else if(syn480DataInfo.valid)
			{
				nDataTimes++;
				nBytes=syn480_GetData(dataBuff);
				printf("�յ� %d �ֽ�����,�ֽ��������� %d ",nBytes,syn480DataInfo.nBytesReceivedPre);
				if(nBytes>0)
				{
					for(i=0;i<nBytes;i++)
					{
						if(dataBuff[i]+syn480DataInfo.dataBufferPre[i]!=0xff)
							break;
					}
					if(i<nBytes)
					{
						printf("�������󣬴���λ�� %d ",i+1);
						nDataError++;
					}
					printf("\r\n");
						
					for(i=0;i<nBytes;i++)
						printf("%3d ",dataBuff[i]);
					printf("\r\n");
					for(i=0;i<nBytes;i++)
						printf("%3d ",syn480DataInfo.dataBufferPre[i]);
					
					printf("\r\n\r\n");
				}
				LCD_ShowAsciiString(0,0,(u8*)"Press Key0 to show Debug",2,RED,GRAY);
			}
			
			
			
			
          
			LED_RUN();
			
				
				
				
				
					
			POINT_COLOR=RED;	  
		  
			sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//��LCD ID��ӡ��lcd_id���顣 
			LCD_ShowString(30,110,200,16,16,lcd_id);		//��ʾLCD ID	      					 
			//LCD_ShowString(30,130,200,12,12,"2015/1/14");	  
			LCD_ShowCnString(0,200,(u8*)"��������������������һ������������ʱ�����������ҵ���",2,RED,GRAY);
			sprintf((char*)lcd_id,"nDataError=%10d",nDataError);
			LCD_ShowAsciiString(0,32,lcd_id,2,RED,YELLOW);
			sprintf((char*)lcd_id,"nDataTimes=%10d",nDataTimes);
			LCD_ShowAsciiString(0,64,lcd_id,2,RED,YELLOW);
			
			sprintf((char*)lcd_id,"nSysTick=%5d",nSysTick2);
			LCD_ShowAsciiString(0,200+64,lcd_id,2,RED,YELLOW);
			sprintf((char*)lcd_id,"nTIM2_CCR=%5d",nTIM2_CCR);
			LCD_ShowAsciiString(0,200+96,lcd_id,2,RED,YELLOW);
			
			LCD_ShowAsciiString(0,500,(u8*)"abcDSE%^NNbbbbb",2,RED,GRAY);
						
				
	
		  
	  
	  }
}

/***********************************END OF FILE*********************************/

