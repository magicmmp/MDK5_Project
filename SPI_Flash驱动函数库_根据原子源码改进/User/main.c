	
#include "stm32f10x.h"

#include "led.h"
#include "sys.h"
#include "lcd.h"
#include "stdlib.h"
#include "font.h" 
#include "w25qxx.h"

//Ҫд�뵽W25Q64���ַ�������
const u8 TEXT_Buffer[]={"ELITE STM32 SPI TEST"};
#define SIZE sizeof(TEXT_Buffer)


int main()
{		

	u8 x=0;
	u8 lcd_id[64];			//���LCD ID�ַ���
	u32 i;
	u8 datatemp[SIZE];
	u32 FLASH_SIZE; 
	
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	
	 if(SysTick_Config(SystemCoreClock/1000))	 	/* 1ms */
	 { 
		/* Capture error */ 
		 while (1);
	 }
	 LED_Init();
	 W25QXX_Init();			//W25QXX��ʼ�� 
	 LCD_Init();

	while(W25QXX_ReadID()!=W25Q128)								//��ⲻ��W25Q128
	{	
		LCD_ShowAsciiString(0,0,(u8*)"W25Q128 Check Failed!",2,BLUE,GRAY);
		LCD_ShowAsciiString(0,128,(u8*)"W25Q128 Check Failed!",2,RED,GRAY);	
	}

  LCD_ShowAsciiString(0,160,(u8*)"W25Q128 Ready!",2,RED,GRAY);	
	FLASH_SIZE=128*1024*1024;	//FLASH ��СΪ16M�ֽ�
 	POINT_COLOR=BLUE;//��������Ϊ��ɫ	  
	
	LCD_ShowAsciiString(0,200,(u8*)"Start Write W25Q128....",2,RED,GRAY);	
	W25QXX_Write((u8*)TEXT_Buffer,FLASH_SIZE-100,SIZE);			//�ӵ�����100����ַ����ʼ,д��SIZE���ȵ�����
	LCD_ShowAsciiString(0,250,(u8*)"W25Q128 Write Finished!",2,RED,GRAY);	//��ʾ�������
	
	
	LCD_ShowAsciiString(0,300,(u8*)"Start Read W25Q128.... ",2,RED,GRAY);	
	W25QXX_Read(datatemp,FLASH_SIZE-100,SIZE);					//�ӵ�����100����ַ����ʼ,����SIZE���ֽ�
	
	LCD_ShowAsciiString(0,350,(u8*)"The Data Readed Is:  ",2,RED,GRAY);	
	
	LCD_ShowAsciiString(0,400,datatemp,2,RED,GRAY);	//��ʾ�������ַ���
	 
	
	  while (1)
	  {
          
			if(led_time_count_tmp==0)
			{
				led_time_count_tmp=led_time_count;
				LED0=!LED0;
				LED1=!LED0;				
				
				if(1)
				{				
					LCD_ShowCnString(0,500,(u8*)"��������������������һ������������ʱ�����������ҵ���",2,RED,GRAY);
					LCD_ShowAsciiString(0,700,(u8*)"abcDSE%^NNbbbbb",2,RED,GRAY);
				
				}
	
			 }  
	  
	  }
}

/***********************************END OF FILE*********************************/

