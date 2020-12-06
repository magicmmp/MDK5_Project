	
#include "stm32f10x.h"

#include "led.h"
#include "sys.h"
#include "lcd.h"
#include "stdlib.h"
#include "font.h" 

int main()
{		

	u8 x=0;
	u8 lcd_id[64];			//存放LCD ID字符串
	u32 i;
	
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	
	 if(SysTick_Config(SystemCoreClock/1000))	 	/* 1ms */
	 { 
		/* Capture error */ 
		 while (1);
	 }
	 LED_Init();
	 
	LCD_Init();
	POINT_COLOR=RED;
	
	

	 
	 
	
	
	
	
	 
	
	  while (1)
	  {
          
			if(led_time_count_tmp==0)
			{
				led_time_count_tmp=led_time_count;
				LED0=!LED0;
				LED1=!LED0;
				
				
				if(1)
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
					POINT_COLOR=RED;	  
				//	LCD_ShowString(30,40,210,24,24,"Elite STM32F1 ^_^"); 
					LCD_ShowString(30,70,200,16,16,"TFTLCD TEST");
					LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
					sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//将LCD ID打印到lcd_id数组。 
					LCD_ShowString(30,110,200,16,16,lcd_id);		//显示LCD ID	      					 
					//LCD_ShowString(30,130,200,12,12,"2015/1/14");	  
					LCD_ShowCnString(0,200,(u8*)"我们日历年月星期设置一二三们四五六时分秒上下左右电子",2,RED,GRAY);
					
					sprintf((char*)lcd_id,"nTime=%04d",nTime);
					LCD_ShowAsciiString(0,200+64,lcd_id,2,RED,YELLOW);
					
					LCD_ShowAsciiString(0,500,(u8*)"abcDSE%^NNbbbbb",2,RED,GRAY);
						x++;
					if(x==12)
						x=0;
				}
				else
				{
					
					LCD_SetCursor(240,400);      				//设置光标位置 
					LCD_WriteRAM_Prepare();     			//开始写入GRAM	  
					for(i=0;i<128000;i++)//从左到右填充一行
						LCD->LCD_RAM=GRAY;	//显示颜色 	    
					
				}
				
				
						   		
				
				
				
				
			 }
			
			 
		 
		
		  
	  
	  }
}

/***********************************END OF FILE*********************************/

