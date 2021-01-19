	
#include "stm32f10x.h"
#include "key.h"
#include "led.h"
#include "sys.h"
#include "lcd.h"
#include "stdlib.h"
#include "font.h" 
#include "timer2.h"
#include "delay.h"

int main()
{		

	u8 key; //����ֵ
	u8 x=0;
	u8 lcd_id[64];			//���LCD ID�ַ���
	u32 i;;
	
	//SCB->VTOR = FLASH_BASE | 0x10000;
	
	
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	
	 if(SysTick_Config(SystemCoreClock/1000))	 	/* 1ms */
	 { 
		/* Capture error */ 
		 while (1);
	 }
	 KEY_Init();
	 LED_Init();
	 
	LCD_Init();
	POINT_COLOR=RED;
	LCD_Clear(BLACK);

	TIM2_PWM_Init();
	
	
	

	 
	 
	
	
	
	
	 
	
	  while (1)
	  {
			LED_RUN();
			
			key=KEY_Scan(0);	//�õ���ֵ
	   	if(key)
			{						   
				switch(key)
				{				 
					case WKUP_PRES:	//���Ʒ�����
						NECSendByte();
						LCD_ShowAsciiString(0,200+180,(u8*)"WKUP_PRES",2,RED,YELLOW);
						break; 
					case KEY1_PRES:	//����LED1��ת	 
						LED1=!LED1;
						LCD_ShowAsciiString(0,200+180,(u8*)"KEY1_PRES",2,RED,YELLOW);
						break;
					case KEY0_PRES:	//ͬʱ����LED0,LED1��ת 
						LED0=!LED0;
						LED1=!LED0;
						LCD_ShowAsciiString(0,200+180,(u8*)"KEY0_PRES",2,RED,YELLOW);
						break;
				}
      }
			//TestSignal();
			//NECSendByte();
		
				
				//LED0=!LED0;
				//LED1=!LED0;
				
				
				if(1)
				{
					if(0)
					{
						switch(x)
						{
							case 0:LCD_Clear(WHITE);break;
							case 1:LCD_Clear(BLACK);break;
							case 2:LCD_Clear(BLUE);break;
							case 3:LCD_Clear(RED);break;
							case 4:LCD_Clear(MAGENTA);break;
							case 5:LCD_Clear(GREEN);break;
							case 6:LCD_Clear(CYAN);break; 
							case 7:LCD_Clear(YELLOW);break;
							case 8:LCD_Clear(BRRED);break;
							case 9:LCD_Clear(GRAY);break;
							case 10:LCD_Clear(LGRAY);break;
							case 11:LCD_Clear(BROWN);break;
						}
					}
					
						
					x++;
					if(x==12)
						x=0;
					
				 
					LCD_ShowString(30,110,200,16,16,lcd_id);		//��ʾLCD ID	      					 
					//LCD_ShowString(30,130,200,12,12,"2015/1/14");	  
					LCD_ShowCnString(0,200,(u8*)"��������������������һ������������ʱ�����������ҵ���",2,RED,GRAY);
					
					sprintf((char*)lcd_id,"nTime=%04d",nTime);
					LCD_ShowAsciiString(0,200+64,lcd_id,2,RED,YELLOW);
					
					sprintf((char*)lcd_id,"test_time2=%05d",test_time2);
					LCD_ShowAsciiString(0,200+128,lcd_id,2,RED,YELLOW);
					
					
						
				}
				
				
				

		  
	  
	  }
}

/***********************************END OF FILE*********************************/

